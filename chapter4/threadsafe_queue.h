//
// Created by 吴凡 on 2017/2/24.
//

#ifndef CONCURRENCYINACTION_THREADSAFE_QUEUE_H
#define CONCURRENCYINACTION_THREADSAFE_QUEUE_H

/*
 *
 * namespace std {
 *
 * template<class T, class Container=deque<T>>
 * class queue {
 * public:
 *      explicit queue(const Container&);
 *      explicit queue(Container&& container=Container());
 *
 *      template<class Alloc>
 *      explicit queue(const Alloc&);
 *      template<class Alloc>
 *      queue(const Container&, const Alloc&);
 *      template<class Alloc>
 *      queue(Container&&, const Alloc&);
 *      template<class Alloc>
 *      queue(queue&&, const Alloc&);
 *
 *      void swap(queue& q);
 *      bool empty() const;
 *      size_type size() const;
 *
 *      T& front();
 *      const T& front() const;
 *
 *      T& back();
 *      const T& back() const;
 *
 *      void push(const T&);
 *      void push(T&&);
 *
 *      void pop();
 *
 *      template<class... Args>
 *      void emplace(Args&&... args);
 *      ...
 * };
 *
 *
 * }
 *
 */

#include <memory>
#include <queue>
#include <mutex>
#include "../nocopyable.h"

template<class T>
class threadsafe_queue : protected nocopyable{
public:
    threadsafe_queue();

    void push(T);

    bool try_pop(T&);
    std::shared_ptr<T> try_pop();

    void wait_then_pop(T&);
    std::shared_ptr<T> wait_then_pop();

    bool empty() const;
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
};


#endif //CONCURRENCYINACTION_THREADSAFE_QUEUE_H
