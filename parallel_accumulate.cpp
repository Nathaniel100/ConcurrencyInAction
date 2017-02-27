//
// Created by 吴凡 on 2017/2/21.
//

#include <iterator>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>

template<class Iterator, class T>
struct Accumulator {
    void operator() (Iterator first, Iterator last, T &result) {
        result = std::accumulate(first, last, result);
    }
};

template<class Iterator, class T>
T parallel_accumulate(Iterator first, Iterator last, T initial) {
    const unsigned distance = (const unsigned int) std::distance(first, last);
    if(!distance) return initial;
    const unsigned cores = std::thread::hardware_concurrency();
    const unsigned threadNumbers = cores == 0 ? 2 : cores;
    unsigned blocks = distance / threadNumbers;

    std::vector<std::thread> threads(threadNumbers - 1);
    std::vector<T> results(threadNumbers);
    Iterator start = first;
    Iterator end = start;
    for(unsigned i = 0; i < threadNumbers - 1; i++) {
        std::advance(end, blocks);
        threads[i] = std::thread{Accumulator<Iterator, T>(), start, end, std::ref(results[i])};
        start = end;
    }
    Accumulator<Iterator, T>()(start, last, results[threadNumbers - 1]);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return std::accumulate(results.begin(), results.end(), initial);
};

int main() {
    std::vector<int> v(1000, 10);
    int accuV = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Accumulate v: " << accuV << '\n';

    int parallelAccuV = parallel_accumulate(v.begin(), v.end(), 0);
    std::cout << "Parallel Accumulate v: " << parallelAccuV << '\n';
}