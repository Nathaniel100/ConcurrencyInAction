//
// Created by 吴凡 on 2017/2/24.
//


/*
 *
 *
 * template<class Function, class... Args>
 * std::future<typename std::result_of<Function(Args...)>::type> async(Function&&, Args&&...);
 *
 * template<class Function, class... Args>
 * std::future<typename std::result_of<Function(Args...)>::type> async(std::launch policy, Function&&, Args&&...);
 *
 */

#include <vector>
#include <numeric>
#include <future>
#include <iostream>

template<class RandIter>
int ParallelAccumulate(RandIter begin, RandIter end) {
    auto length = std::distance(begin, end);
    if(length < 1000) {
        return std::accumulate(begin, end, 0);
    }

    RandIter mid = begin;
    std::advance(mid, length / 2);
    auto handle = std::async(std::launch::async, ParallelAccumulate<RandIter>, mid, end);
    auto result = ParallelAccumulate(begin, mid);
    return result + handle.get();
}

int main() {
    std::vector<int> v(100000, 10);
    std::cout << "Sum: " << ParallelAccumulate(v.begin(), v.end()) << "\n";
}

