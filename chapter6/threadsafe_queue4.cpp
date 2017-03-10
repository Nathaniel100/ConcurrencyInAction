//
// Created by 吴凡 on 2017/3/8.
//

#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include "../nocopyable.h"

template<class T>
class threadsafe_queue : protected nocopyable{
private:
    struct node {
        std::shared_ptr<T> data_;
        std::unique_ptr<node> next_;
    };
public:
    threadsafe_queue():head_{new node}, tail_{head_.get()}{}
    void push(T value) {
        std::shared_ptr<T> data = std::make_shared<T>(std::move(value));
        std::unique_ptr<node> p(new node);
        {
            std::lock_guard <std::mutex> lock(tailMutex_);
            tail_->data_ = data;
            node *new_tail = p.get();
            tail_->next_ = std::move(p);
            tail_ = new_tail;
        }
        cond_.notify_one(); // does not need hold a lock
    }

    std::shared_ptr<T> try_pop() {
        std::unique_ptr<node> old_head = try_pop_head();
        return old_head ? old_head->data_ : std::shared_ptr<T>();
    }

    bool try_pop(T& result) {
        std::unique_ptr<node> old_head = try_pop_head(result);
        return !!old_head;
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_ptr<node> old_head = wait_pop_head();
        return old_head->data_;
    }

    void wait_and_pop(T& result) {
        wait_pop_head(result);
    }

    bool empty() {
        std::lock_guard<std::mutex> headLock(headMutex_);
        return (head_.get() == get_tail());
    }

private:

    std::unique_ptr<node> wait_pop_head() {
        std::unique_lock<std::mutex> headLock = wait_for_data();
        return pop_head();
    }

    std::unique_ptr<node> wait_pop_head(T& result) {
        std::unique_lock<std::mutex> headLock = wait_for_data();
        result = std::move(*head_->data_);
        return pop_head();
    }

    std::unique_lock<std::mutex> wait_for_data() {
        std::unique_lock<std::mutex> headLock(headMutex_);
        cond_.wait(headLock, [this]{return head_.get() != get_tail();});
        return std::move(headLock);
    }

    std::unique_ptr<node> pop_head() {
        std::unique_ptr<node> old_head = std::move(head_);
        head_ = std::move(old_head->next_);
        return std::move(old_head);
    }

    std::unique_ptr<node> try_pop_head() {
        std::lock_guard<std::mutex> headLock(headMutex_);
        if(head_.get() == get_tail()) return std::unique_ptr<node>();
        return pop_head();
    }

    std::unique_ptr<node> try_pop_head(T& result) {
        std::lock_guard<std::mutex> headLock(headMutex_);
        if(head_.get() == get_tail()) return std::unique_ptr<node>();
        result = std::move(*head_->data_);
        return pop_head();
    }

    const node* get_tail() {
        std::lock_guard<std::mutex> lock(tailMutex_);
        return tail_;
    }

    std::unique_ptr<node> head_;
    node* tail_;
    std::mutex headMutex_;
    std::mutex tailMutex_;
    std::condition_variable cond_;
};

struct data_chunk {
    int value_;
    bool last_;
    data_chunk(int value = 0, bool last = false): value_{value}, last_{last} {}
};
constexpr int totalChunks = 1000;
int preparedChunkCount = 0;

bool more_data_to_prepare() {
    return preparedChunkCount != totalChunks;
}

data_chunk prepare_data() {
    preparedChunkCount++;
    return data_chunk(preparedChunkCount, preparedChunkCount == totalChunks);
}

std::mutex coutMutex_;
void process_data(const data_chunk& chunk) {
    std::lock_guard<std::mutex> lock(coutMutex_);
    std::cout << "ChunK[" << chunk.value_ << "]\n";
}
bool is_last_chunk(const data_chunk& chunk) {
    return chunk.last_;
}

threadsafe_queue<data_chunk> chunks;

void data_preparation_thread() {
    while(more_data_to_prepare()) {
        data_chunk data = prepare_data();
        chunks.push(std::move(data));
    }
}

void data_processing_thread() {
    while(true) {
        data_chunk data;
        chunks.wait_and_pop(data);
        process_data(data);
        if(is_last_chunk(data)) break;
    }
}

int main() {
    std::thread t1{data_preparation_thread};
    std::thread t2{data_processing_thread};
    t1.join();
    t2.join();

    std::cout << "Chunks is empty ? " << std::boolalpha << chunks.empty() << "\n";
    return 0;
}
