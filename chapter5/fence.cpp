//
// Created by 吴凡 on 2017/3/6.
//

#include <atomic>
#include <thread>
#include <cassert>

bool x = false;
std::atomic<bool> y;
std::atomic<int> z;

void write_x_then_y() {
    x = true;
    std::atomic_thread_fence(std::memory_order_release); // synchronized-with (2)
    y.store(true, std::memory_order_relaxed);
}


void read_y_then_x() {
    while(!y.load(std::memory_order_relaxed));
    std::atomic_thread_fence(std::memory_order_acquire); // synchronized-with (1)
    if(x) z++;
}


int main() {
    x = false;
    y = false;
    z = 0;
    std::thread t1{write_x_then_y};
    std::thread t2{read_y_then_x};
    t1.join();
    t2.join();
    assert(z.load() != 0); // not fire
}