//
// Created by 吴凡 on 2017/3/3.
//


#include <atomic>
#include <vector>
#include <thread>
#include <iostream>
#include <chrono>


// memory model: happens-before and synchronizes-with

std::atomic<bool> ready{false};
std::vector<int> data;


void reader_thread() {
    while(!ready.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "Data: " << data[0] << "\n"; // Must be 42
}


void writer_thread() {
    ready.store(true);
    data.push_back(42);
}

int main() {
    std::thread t1{reader_thread};
    std::thread t2{writer_thread};
    t1.join();
    t2.join();
    return 0;
}
