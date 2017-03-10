//
// Created by 吴凡 on 2017/3/10.
//


#include <functional>
#include <list>
#include <vector>

#include <boost/thread/shared_mutex.hpp>
#include <thread>
#include "../nocopyable.h"

template<class Key, class Value, class Hash = std::hash<Key>>
class LookupTable : private nocopyable {
private:
    class Bucket {
    private:
        using BucketValue = std::pair<Key, Value>;
        using BucketData = std::list<BucketValue>;
        using BucketIterator = typename BucketData::iterator;
        BucketData data_;
        mutable boost::shared_mutex mutex_;

        BucketIterator FindEntryFor(const Key& key) {
            return std::find_if(data_.begin(), data_.end(), [&](const BucketValue& value){
                return value.first == key;
            });
        }
        friend class LookupTable;
    public:
        Value ValueFor(const Key& key, const Value& defaultValue = Value()) {
            boost::shared_lock<boost::shared_mutex> readLock(mutex_);
            BucketIterator iterator = FindEntryFor(key);
            return iterator == data_.end() ? defaultValue : iterator->second;
        }

        void AddOrUpdateValue(const Key& key, const Value& value) {
            std::unique_lock<boost::shared_mutex> lock(mutex_);
            BucketIterator  iterator = FindEntryFor(key);
            if(iterator == data_.end()) {
                data_.push_back(BucketValue(key, value));
            } else {
                iterator->second = value;
            }
        }

        void RemoveKey(const Key& key) {
            std::unique_lock<boost::shared_mutex> lock(mutex_);
            BucketIterator iterator = FindEntryFor(key);
            if(iterator != data_.end()) {
                data_.erase(iterator);
            }
        }
    };

public:
    using key_type = Key;
    using value_type = Value;
    using hash_type = Hash;

    LookupTable(unsigned int bucketNumber = 19, Hash hash = Hash()): buckets_(bucketNumber), hash_(hash) {
        for(unsigned int i = 0; i < bucketNumber; ++i) {
            buckets_[i].reset(new Bucket);
        }
    }

    Value ValueFor(const Key& key, const Value& defaultValue = Value()) const {
        return GetBucket(key).ValueFor(key, std::move(defaultValue));
    }

    void AddOrUpdateValue(const Key& key, const Value& value) {
        return GetBucket(key).AddOrUpdateValue(key, value);
    }

    void RemoveKey(const Key& key) {
        return GetBucket(key).RemoveKey(key);
    }

    std::map<Key, Value> ToMap() const {
        std::vector<std::unique_lock<boost::shared_mutex>> locks;
        for(int i = 0; i < buckets_.size(); i++) {
            locks.push_back(std::unique_lock<boost::shared_mutex>(buckets_[i]->mutex_));
        }

        std::map<Key, Value> res;
        for(auto &it : buckets_) {
            for(auto it2 = it->data_.begin(); it2 != it->data_.end(); ++it2) {
                res.insert(*it2);
            }
        }
        return res;
    };

private:
    Bucket& GetBucket(const Key& key) const {
        return *buckets_[hash_(key)%buckets_.size()];
    }
    std::vector<std::unique_ptr<Bucket>> buckets_;
    Hash hash_;
};

LookupTable<std::string, int> table;
void table_add1() {
    std::string s("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    for(auto c : s) {
        table.AddOrUpdateValue(std::string(1, c), (c - 'A'));
    }
    for(auto c : s) {
        int v = table.ValueFor(std::string(1, c), -1);
        assert(v == (c - 'A'));
    }
}

void table_add2() {
    std::string s("abcdefghijklmnopqrstuvwxyz");
    for(auto c : s) {
        table.AddOrUpdateValue(std::string(1, c), (c - 'a'));
    }
    for(auto c : s) {
        int v = table.ValueFor(std::string(1, c), -1);
        assert(v == (c - 'a'));
    }
}

std::mutex cout_mutex;
void print_table() {
    std::map<std::string, int> m = table.ToMap();
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "{";
    for(auto &i : m) {
        std::cout << i.first << ":" << i.second << ", ";
    }
    std::cout << "}\n";
}


int main() {
    std::thread t1{table_add1};
    std::thread t2{table_add2};
    t1.join();
    t2.join();

    std::thread t3{print_table};
    std::thread t4{print_table};
    t3.join();
    t4.join();


    return 0;
}