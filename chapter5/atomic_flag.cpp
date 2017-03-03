//
// Created by 吴凡 on 2017/3/3.
//
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
#include "../nocopyable.h"


/*
 * namespace std {
 * class atomic_flag : private nocopyable{
 * public:
 *      atomic_flag();
 *      ~atomic_flag();
 *
 *      void clear(std::memory_order order);
 *      bool test_and_set(std::memory_order order);
 * };
 *
 * #define ATOMIC_FLAG_INIT implementation-defined
 *
 * }
 *
 */

// atomic_flag used as spin-lock mutex

class spinlock_mutex : private nocopyable {
public:
    spinlock_mutex():flag_(ATOMIC_FLAG_INIT) {
    }
    void lock() {
        while(flag_.test_and_set(std::memory_order_acquire)); // spin
    }
    void unlock() {
        flag_.clear(std::memory_order_release);
    }
private:
    std::atomic_flag flag_;
};

spinlock_mutex spinlockMutex;

void f(int n)
{
    for (int cnt = 0; cnt < 100; ++cnt) {
        std::lock_guard<spinlock_mutex> lock(spinlockMutex);
        std::cout << "Output from thread " << n << "\n";
    }
}

int main() {
    std::vector<std::thread> threads;
    for(int i = 0; i < 10; i++) {
        threads.emplace_back(f, i);
    }
    for(auto& t : threads) {
        t.join();
    }
}