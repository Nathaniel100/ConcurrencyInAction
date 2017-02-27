//
// Created by 吴凡 on 2017/2/24.
//


#include <vector>
#include <numeric>
#include <future>
#include <iostream>

/*
 *
 * template<class R> class promise;
 * template<class R> class promise<R&>;
 * template<> class promise<void>;
 *
 * template<class R>
 * class promise {
 * public:
 *      promise();
 *      promise(const promise&) = delete;
 *      promise(promise&&);
 *
 *      std::future<R> get_future();
 *
 *      void set_value(const R&);
 *      void set_value(R&&);
 *
 *      void set_exception(std::exception_ptr);
 * };
 *
 */


void accumulate(std::vector<int>::iterator begin, std::vector<int>::iterator end, std::promise<int> result) {
    int sum = std::accumulate(begin, end, 0);
    result.set_value(sum); // notify future
}

void do_work(std::promise<void> barrier) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    barrier.set_value();
}

int main() {
    // Demonstrate using promise<int> to transmit a result between threads.
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    std::thread thread1{accumulate, v.begin(), v.end(), std::move(promise)};
    future.wait();
    std::cout << "Sum: " << future.get() << "\n";
    thread1.join();

    // Demonstrate using promise<void> to signal state between threads.
    std::promise<void> barrier;
    std::future<void> barrier_future = barrier.get_future();
    std::thread thread2{do_work, std::move(barrier)};
    barrier_future.wait();
    thread2.join();
}