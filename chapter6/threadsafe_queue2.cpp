//
// Created by 吴凡 on 2017/3/7.
//


#include <memory>
#include <mutex>
#include <stack>
#include <queue>
#include <iostream>
#include <thread>
#include "../nocopyable.h"

template<class T>
class threadsafe_queue : protected nocopyable{
public:
    void push(T t) {
        std::shared_ptr<T> p(std::make_shared<T>(std::move(t))); // allocate outside
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(p);
        cond_.notify_one();
    }

    void wait_then_pop(T& result) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]{return !queue_.empty();});
        std::shared_ptr<T> front = queue_.front();
        queue_.pop();
        result = std::move(*front);
    }

    std::shared_ptr<T> wait_then_pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]{return !queue_.empty();});
        std::shared_ptr<T> front = queue_.front();
        queue_.pop();
        return front;
    }

    bool try_pop(T& result) {
        std::lock_guard<std::mutex> lock(mutex_);
        if(queue_.empty()) return false;
        result = std::move(*queue_.front());
        queue_.pop();
        return true;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if(queue_.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> front = queue_.front();
        queue_.pop();
        return front;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }
private:
    mutable std::mutex mutex_;
    std::queue<std::shared_ptr<T>> queue_; // hold std::shared_ptr
    std::condition_variable cond_;
};

struct data_chunk {
    int value_;
    bool last_;
    data_chunk(int value = 0, bool last = false): value_{value}, last_{last} {}
};
constexpr int totalChunks = 10;
int preparedChunkCount = 0;

bool more_data_to_prepare() {
    return preparedChunkCount != totalChunks;
}

data_chunk prepare_data() {
    preparedChunkCount++;
    return data_chunk(preparedChunkCount, preparedChunkCount == totalChunks);
}

void process_data(const data_chunk& chunk) {
    std::cout << "ChunK[" << chunk.value_ << "]\n";
}
bool is_last_chunk(const data_chunk& chunk) {
    return chunk.last_;
}

threadsafe_queue<data_chunk> chunks;

void data_preparation_thread() {
    while(more_data_to_prepare()) {
        data_chunk data = prepare_data();
        chunks.push(data);
    }
}

void data_processing_thread() {
    while(true) {
        data_chunk data;
        chunks.wait_then_pop(data);
        process_data(data);
        if(is_last_chunk(data)) break;
    }
}

int main() {
    std::thread t1{data_preparation_thread};
    std::thread t2{data_processing_thread};
    t1.join();
    t2.join();

    std::cout << "Chunks is empty ? " << std::boolalpha << chunks.empty() << "\n";
    return 0;
}
