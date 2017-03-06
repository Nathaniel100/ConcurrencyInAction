//
// Created by 吴凡 on 2017/3/6.
//


#include <atomic>
#include <string>
#include <thread>
#include <chrono>
#include <cassert>

struct X {
    int i;
    std::string s;
};

std::atomic<X*> p;
std::atomic<int> a;

void create_x() {
    X* x = new X;
    x->i = 42;
    x->s = "Hello";
    a.store(99, std::memory_order_relaxed);
    p.store(x, std::memory_order_release);
}

void use_x() {
    X *x = nullptr;
    while(!(x = p.load(std::memory_order_consume))) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
    assert(x->i == 42); // not fire
    assert(x->s == "Hello"); // not fire
    assert(a.load(std::memory_order_relaxed) == 99); // can fire
}

int main() {
    std::thread t1{create_x};
    std::thread t2{use_x};
    t1.join();
    t2.join();
    return 0;
}