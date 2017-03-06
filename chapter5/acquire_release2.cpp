//
// Created by 吴凡 on 2017/3/6.
//

#include <atomic>
#include <thread>
#include <cassert>

std::atomic<bool> x(false), y(false);
std::atomic<int> z(0);

void write_x_then_y() {
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_release);
}

void read_y_then_x() {
    while(!y.load(std::memory_order_acquire));
    if(x.load(std::memory_order_relaxed))
        z++;
}

int main() {
    std::thread t1{write_x_then_y};
    std::thread t2{read_y_then_x};

    t1.join();
    t2.join();
    assert(z.load() != 0); // cannot fire
    return 0;
}

