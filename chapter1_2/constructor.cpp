//
// Created by 吴凡 on 2017/2/21.
//

#include <iostream>
#include <thread>

/*
 * namespace std {
 * class thread {
 * public:
 *      thread();
 *      thread(thread&& other);
 *      template<class Function, class... Args>
 *      thread(Function&& f, Args&&... args);
 *      thread(const thread&) = delete;
 * };
 * }
 *
 * Parameters:
 * other    : another thread object
 * f        : Callable function object
 * args...  : Arguments to pass to the function
 *            Moved or copied by value
 *            if a preference argument needs to be passed to the thread function, it has to be wrapped with std::ref or std::cref )
 */

struct BackgroundTask {
    void operator()() {
        // do something
        std::cout << "Background task...\n";
    }
};

// Ensure that thread accesses to valid object before it finishes.
struct func {
    int &i_;
    func(int &i):i_{i} {}
    void operator()() {
        for(int i = 0; i < 1000000; ++i) {
            if(i % 999 == 0) {
                std::cout << i_ << "--" << i << "\n";
            }
        }
    }
};

void oops() {
    int state = 10;
    std::thread t{func(state)};
    t.detach();
}

// Normally call join or detach immediately after thread object is constructed.
// But wait do something may produce some error. Join must be called in catch block.

void do_something() {
    // do_something
}

void f() {
    int state = 10;
    std::thread t{func(state)};

    try {
        do_something();
    } catch (...) {
        t.join(); // it's necessary
        throw;
    }

    t.join();
}

// Using RAII to manage thread object
class ThreadGuard {
public:
    ThreadGuard(std::thread &t): t_{t} {}
    ~ThreadGuard() {
        if(t_.joinable()) {
            t_.join();
        }
    }
private:
    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
    std::thread &t_;
};

void f2() {
    int state = 10;
    std::thread t{func(state)};
    ThreadGuard guard{t};
    do_something();
}


int main() {
    BackgroundTask task1;
    std::thread t1{task1};
    std::thread t2{[](){
        std::cout << "Lambda task...\n";
    }};
    t1.join(); // decide to join or detach before the thread object is destroyed
    t2.join();

    oops();
    getchar();
}
