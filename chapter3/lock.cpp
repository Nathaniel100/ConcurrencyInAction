//
// Created by 吴凡 on 2017/2/22.
//

#include <mutex>
#include <string>

struct SomeBigObject {
    std::string s;
};

void swap(SomeBigObject &l, SomeBigObject &r) {
    std::swap(l.s, r.s);
}

class X {
private:
    SomeBigObject detail_;
    std::mutex mutex_;
public:
    friend void swap(X &l, X &r) {
        if(&l == &r) return;
        std::lock(l.mutex_, r.mutex_); // lock two mutexes
        // adopt_lock indicates to the lock_guard object that has been locked,
        // they should just adopt the ownership of the existing lock on the mutex
        // rather than attempt lock the mutex on the constructor
        std::lock_guard lock1(l.mutex_, std::adopt_lock);
        std::lock_guard lock2(r.mutex_, std::adopt_lock); // same up
        swap(l.detail_, r.detail_);
    }
};

// unique_lock

class X1 {
private:
    SomeBigObject detail_;
    std::mutex mutex_;
public:
    friend void swap(X1 &l, X1 &r) {
        if(&l == &r) return;
        std::unique_lock lock1(l.mutex_, std::defer_lock);
        std::unique_lock lock2(r.mutex_, std::defer_lock);
        std::lock(lock1, lock2);
        swap(l.detail_, r.detail_);
    }
};

// Transfer mutex ownership

std::mutex some_mutex;
void prepare_data() {
}
void do_something() {
}

std::unique_lock<std::mutex> getLock() {
    extern std::mutex some_mutex;
    std::unique_lock ul(some_mutex);
    prepare_data();
    return ul;
}

void process_data() {
    std::unique_lock<std::mutex> ul(getLock());
    do_something();
}

// Lock granularity
// One lock should be held for the minimum possible time needed to perform the required operations
/*
void get_and_process_data()
{
    std::unique_lock<std::mutex> my_lock(the_mutex);
    some_class data_to_process=get_next_data_chunk();
    my_lock.unlock();   // don't need mutex locked in process
    result_type result=process(data_to_process);
    my_lock.lock();     // relock mutex to write result
    write_result(data_to_process,result);
}
 */

class Y {
private:
    int cheap_;
    mutable std::mutex mutex_;

    int getCheapDetail() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return cheap_;
    }
public:
    Y(int cheap): cheap_{cheap} {}
    friend bool operator==(Y& l, Y& r) {
        if(&l == &r) return true;
        // hold one lock at a time because int copy is cheap
        int lc = l.getCheapDetail();
        int rc = r.getCheapDetail();
        return lc == rc;
    }
};


int main() {

}