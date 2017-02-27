//
// Created by 吴凡 on 2017/2/22.
//

#include <iostream>
#include <thread>
#include "threadsafe_stack.h"

template<class T>
threadsafe_stack<T>::threadsafe_stack() {
}

template<class T>
threadsafe_stack<T>::threadsafe_stack(const threadsafe_stack &t) {
    std::lock_guard<std::mutex> lock(t.mutex_);
    stack_ = t.stack_;
}

template<class T>
void threadsafe_stack<T>::push(T t) {
    std::lock_guard<std::mutex> lock(mutex_);
    stack_.push(t);
}

template<class T>
std::shared_ptr<T> threadsafe_stack<T>::pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    if(stack_.empty()) throw empty_stack();
    std::shared_ptr<T> p = std::make_shared<T>(stack_.top());
    stack_.pop();
    return p;
}

template<class T>
void threadsafe_stack<T>::pop(T &t) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(stack_.empty()) throw empty_stack();
    t = stack_.top();
    stack_.pop();
}

template<class T>
bool threadsafe_stack<T>::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return stack_.empty();
}

threadsafe_stack<int> ts;

void f() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int k = 0;
    if(!ts.empty()) {
        ts.pop(k);
        std::cout << "k: " << k << '\n';
    }
}

void foo() {
    ts.push(1);
    ts.push(2);

    std::thread t1{f};
    std::thread t2{f};
    t1.join();
    t2.join();
}

int main() {

    foo();
}





