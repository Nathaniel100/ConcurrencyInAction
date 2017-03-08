//
// Created by 吴凡 on 2017/3/8.
//

#include <memory>
#include <iostream>
#include "../nocopyable.h"

// preallocating a dummy node with no data to ensure that there’s always at least one node in the queue
// to separate the node being accessed at the head from that being accessed at the tail.
template<class T>
class queue : protected nocopyable{
public:
    queue():head_{new node}, tail_{head_.get()}{}
    void push(T value) {
        std::shared_ptr<T> data = std::make_shared<T>(std::move(value));
        std::unique_ptr<node> p(new node);
        tail_->data_ = data;
        node *new_tail = p.get();
        tail_->next_ = std::move(p);
        tail_ = new_tail;
    }

    std::shared_ptr<T> try_pop() {
        if(tail_ == head_.get()) return std::shared_ptr<T>();
        std::shared_ptr<T> res(head_->data_);
        const std::unique_ptr<node> old_head = std::move(head_);
        head_ = std::move(old_head->next_);
        return res;
    }

private:
    struct node {
        std::shared_ptr<T> data_;
        std::unique_ptr<node> next_;
    };
    std::unique_ptr<node> head_;
    node* tail_;
};
int main() {
    queue<int> q;
    for (int i = 0; i < 10; ++i) {
        q.push(i);
    }

    for (int i = 0; i < 12; ++i) {
        auto p = q.try_pop();
        if (p) {
            std::cout << "p: " << *p << "\n";
        } else {
            std::cout << "Queue is empty" << "\n";
        }
    }
    for (int i = 0; i < 10; ++i) {
        q.push(i);
    }
    for (int i = 0; i < 12; ++i) {
        auto p = q.try_pop();
        if (p) {
            std::cout << "p: " << *p << "\n";
        } else {
            std::cout << "Queue is empty" << "\n";
        }
    }
    return 0;
}
