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

        std::lock_guard<std::mutex> lock(tailMutex_);
        tail_->data_ = data;
        node *new_tail = p.get();
        tail_->next_ = std::move(p);
        tail_ = new_tail;
    }

    std::shared_ptr<T> try_pop() {
        std::unique_ptr<node> old_head = pop_head();
        return old_head ? old_head->data_ : std::shared_ptr<T>();
    }

private:
    std::unique_ptr<node> pop_head() {
        std::lock_guard<std::mutex> lock(headMutex_);
        if(head_.get() == get_tail()) return std::unique_ptr<node>(); // get_tail must be inside of head mutex lock
        std::unique_ptr<node> old_head = std::move(head_);
        head_ = std::move(old_head->next_);
        return std::move(old_head);
    }
    const node* get_tail() {
        std::lock_guard<std::mutex> lock(tailMutex_);
        return tail_;
    }

    std::unique_ptr<node> head_;
    node* tail_;
    std::mutex headMutex_;
    std::mutex tailMutex_;
};

//################################################################################

struct data_chunk {
    int value_;
    bool last_;
    data_chunk(int value = 0, bool last = false): value_{value}, last_{last} {}
};
constexpr int totalChunks = 10;
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
        chunks.push(data);
    }
}

void data_processing_thread() {
    while(true) {
        std::shared_ptr<data_chunk> data = chunks.try_pop();
        if(!data) continue;
        process_data(*data);
        if(is_last_chunk(*data)) break;
    }
}

int main() {
    std::thread t1{data_preparation_thread};
    std::thread t2{data_processing_thread};
    t1.join();
    t2.join();

    return 0;
}
