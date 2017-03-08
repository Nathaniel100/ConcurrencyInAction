//
// Created by 吴凡 on 2017/3/7.
//

// single thread


#include <memory>
#include <iostream>
#include "../nocopyable.h"


template<class T>
class queue : protected nocopyable {
public:
    void push(T value) {
        std::unique_ptr<node> p(new node(std::move(value)));
        node* new_tail = p.get();
        if (!head_) { // empty
            head_ = std::move(p);
        } else {
            tail_->next_ = std::move(p);
        }
        tail_ = new_tail;
    }

    std::shared_ptr<T> try_pop() {
        if (!head_) return std::shared_ptr<T>();
        std::shared_ptr<T> res = std::make_shared<T>(std::move(head_->data_));
        head_ = std::move(head_->next_);
        return res;
    }

private:
    struct node {
        T data_;
        std::unique_ptr<node> next_;

        node(T data) : data_{std::move(data)}, next_{nullptr} {}
    };

    std::unique_ptr<node> head_;
    node *tail_;
};

int main() {
    std::unique_ptr<int> t(new int(8));
    int *ptr = t.get();
    if (ptr) {
        std::cout << "value: " << *ptr << "\n";
    } else {
        std::cout << "ptr is null\n";
    }
    std::unique_ptr<int> t2 = std::move(t);
    if (ptr) {
        std::cout << "value: " << *ptr << "\n";
    } else {
        std::cout << "ptr is null\n";
    }


    queue<int> q;
    for (int i = 0; i < 10; ++i) {
        q.push(i);
    }

    for (int i = 0; i < 11; ++i) {
        auto p = q.try_pop();
        if (p) {
            std::cout << "p: " << *p << "\n";
        } else {
            std::cout << "Queue is empty" << "\n";
        }
    }
    return 0;
}
