//
// Created by 吴凡 on 2017/2/21.
//


#include <thread>
#include <iostream>
#include <vector>
#include <functional>

void SomeFunction() {
    std::cout << "Thread " << std::this_thread::get_id() << " -- SomeFunction\n";
}

void SomeOtherFunction() {
    std::cout << "Thread " << std::this_thread::get_id() << " -- SomeOtherFunction\n";
}

std::thread f() {
    return std::thread{SomeFunction};
}

std::thread g() {
    return std::thread{SomeOtherFunction};
}

void h(std::thread t) {
    // do something
    t.join();
}

void x() {
    std::thread t = f();
    h(std::move(t));
}

// ScopedThread vs ThreadGuard
class ScopedThread {
public:
    ScopedThread(std::thread t): t_{std::move(t)} {
        if(!t_.joinable())
            throw std::logic_error("No thread");
    }
    ~ScopedThread() {
        t_.join();
    }
private:
    ScopedThread(const ScopedThread &) = delete;
    ScopedThread& operator=(const ScopedThread &) = delete;
    std::thread t_;
};

void DoSomething(int i) {
    std::cout << i << '\n';
}
void spawn() {
    std::vector<std::thread> threads;
    for(int i = 0; i < 20; i++) {
        threads.push_back(std::thread{DoSomething, i});
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int main() {
    std::thread t1{SomeFunction};
    std::thread t2 = std::move(t1);
    t1 = std::thread(SomeOtherFunction);
    std::thread t3;
    t3 = std::move(t2);
    //t1 = std::move(t3); // !! Fatal
    t1.join();
    t3.join();

    x();
    spawn();
}