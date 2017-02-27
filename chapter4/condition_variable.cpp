//
// Created by 吴凡 on 2017/2/24.
//

// std::condition_variable   <==> std::mutex
// std::condition_variable_any  <==> mutex-like

/*
 *
 * namespace std{
 * class condition_variable {
 * public:
 *      condition_variable();
 *      condition_variable(const condition_variable&) = delete;
 *      ~condition_variable();
 *
 *      void wait(std::unique_lock<std::mutex> &lock);
 *      template<class Predicate>
 *      void wait(std::unique_lock<std::mutex> &lock, Predicate pred);
 *
 *      void notify_one();
 *      void notify_all();
 *      ...
 *
 * };
 *
 */


#include <mutex>
#include <thread>
#include <queue>
#include <iostream>

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
    std::cout << "Prepare chunk" << "\n";
    preparedChunkCount++;
    return data_chunk(preparedChunkCount, preparedChunkCount == totalChunks);
}

void process_data(const data_chunk& chunk) {
    std::cout << "Process ChunK[" << chunk.value_ << "]\n";
}
bool is_last_chunk(const data_chunk& chunk) {
    return chunk.last_;
}

std::mutex mutex;
std::queue<data_chunk> chunks;
std::condition_variable cond;


void data_preparation_thread() {
    while(more_data_to_prepare()) {
        data_chunk data = prepare_data();
        std::lock_guard<std::mutex> lock(mutex);
        chunks.push(data);
        cond.notify_one();
    }
}



void data_processing_thread() {
    while(true) {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock, []{return !chunks.empty();});
        data_chunk data = chunks.front();
        chunks.pop();
        lock.unlock();
        process_data(data);
        if(is_last_chunk(data)) break;
    }
}

int main() {
    std::thread t1{data_preparation_thread};
    std::thread t2{data_processing_thread};
    t1.join();
    t2.join();

    std::cout << "Chunks Size: " << chunks.size() << "\n";
    return 0;
}
