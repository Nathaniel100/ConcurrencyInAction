//
// Created by 吴凡 on 2017/2/24.
//


#include <cmath>
#include <future>
#include <iostream>

/*
 *
 * namespace std {
 *
 * template<class T> class packaged_task; // undefined
 *
 * template<class R, class... Args>
 * class packaged_task<R(Args...)> {
 * public:
 *
 *      packaged_task();
 *      template<class Function> explicit packaged_task(Function&&);
 *      packaged_task(const packaged_task&) = delete;
 *      packaged_task(packaged_task&&);
 *
 *      std::future<R> get_future();
 *      void operator()(Args... args);
 *
 * }
 *
 *
 */

int f(int x, int y) {
    return (int) std::pow(x, y);
}

void task_lambda() {
    std::packaged_task<int(int, int)> task([](int x, int y) {
        return (int)std::pow(x, y);}
    );
    std::future<int> future = task.get_future();
    task(2, 9);

    std::cout << "task_lambda:\t" << future.get() << "\n";
}

void task_bind() {
    std::packaged_task<int()> task(std::bind(f, 2, 11));
    std::future<int> future = task.get_future();
    task();

    std::cout << "task_bind: \t" << future.get() << "\n";
}

void task_thread() {
    std::packaged_task<int(int, int)> task(f);
    std::future<int> future = task.get_future();
    std::thread thread(std::move(task), 2, 10);
    thread.join();

    std::cout << "task_thread:\t" << future.get() << "\n";
}


int main() {
    task_lambda();
    task_bind();
    task_thread();
    return 0;
}
