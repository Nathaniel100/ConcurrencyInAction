//
// Created by 吴凡 on 2017/2/22.
//

#ifndef CONCURRENCYINACTION_THREADSAFE_STACK_H
#define CONCURRENCYINACTION_THREADSAFE_STACK_H

#include <exception>
#include <memory>
#include <stack>
#include <mutex>

/*
 * Imagine we designed a class stack, every operation of that is protected by mutex, but the
 * following still exists race conditions.
 *
 * if(!stack.empty()) {
 *      t = stack.top();
 *      stack.pop();
 *      do_something(t);
 * }
 *
 * top and pop race conditions
 */

struct empty_stack : public std::exception {
    const char* what() const throw() {
        return "Empty stack";
    }
};

template<class T>
class threadsafe_stack {
public:
    threadsafe_stack();

    threadsafe_stack(const threadsafe_stack &);

    threadsafe_stack &operator=(const threadsafe_stack &) = delete;

    void push(T);

    std::shared_ptr<T> pop();

    void pop(T &);

    bool empty() const;

private:
    std::stack<T> stack_;
    mutable std::mutex mutex_;
};


#endif //CONCURRENCYINACTION_THREADSAFE_STACK_H
