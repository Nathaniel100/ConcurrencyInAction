//
// Created by 吴凡 on 2017/2/27.
//


#include <list>
#include <iostream>
#include <future>

template<class T> class Type;


template<class ForwardIter>
ForwardIter partition(ForwardIter begin, ForwardIter end) {
    auto& pivot = *begin;
    ForwardIter i = begin;
    ForwardIter j = std::next(begin);
    while(j != end) {
        if(*j < pivot) {
            std::advance(i, 1);
            std::swap(*j, *i);
        }
        std::advance(j, 1);
    }
    std::swap(*i, pivot);
    return i;
}

template<class ForwardIter>
void sqs(ForwardIter begin, ForwardIter end) {
    if(begin == end) return;
    ForwardIter p = partition(begin, end);
    sqs(begin, p);
    ForwardIter q = std::next(p);
    sqs(q, end);
}

template<class T>
std::list<T> sequential_quick_sort1(std::list<T> list) {
    if(list.empty()) return list;
    sqs(list.begin(), list.end());
    return list;
}

// functional programming style
template<class T>
std::list<T> sequential_quick_sort(std::list<T> list) {
    if(list.empty()) return list;
    std::list<T> result;
    result.splice(result.begin(), list, list.begin());
    const T& pivot = *result.begin();
    auto dividePoint = std::partition(list.begin(), list.end(), [&](const T& x) {return x < pivot;});
    std::list<T> lowerList;
    lowerList.splice(lowerList.begin(), list, list.begin(), dividePoint);
    result.splice(result.end(), sequential_quick_sort(std::move(list)));
    result.splice(result.begin(), sequential_quick_sort(std::move(lowerList)));
    return result;
}

// parallel quick sort
template<class T>
std::list<T> parallel_quick_sort(std::list<T> list) {
    if(list.empty()) return list;
    std::list<T> result;
    result.splice(result.begin(), list, list.begin());
    const T& pivot = *result.begin();
    auto dividePoint = std::partition(list.begin(), list.end(), [&](const T& x) {return x < pivot;});
    std::list<T> lowerList;
    lowerList.splice(lowerList.begin(), list, list.begin(), dividePoint);
    std::future<std::list<T>> future = std::async(&parallel_quick_sort<T>, std::move(lowerList));
    result.splice(result.end(), sequential_quick_sort(std::move(list)));
    result.splice(result.begin(), future.get());
    return result;
}

void print(const std::list<int>& list) {
    std::for_each(list.begin(), list.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "\n";
}

int main() {
    std::list<int> l = {5, 1, 9, 8, 4, 7, 3, 2, 0, 6};
    std::list<int> sortedList = sequential_quick_sort1(l);
    print(sortedList);

    std::list<int> sortedList2 = sequential_quick_sort(l);
    print(sortedList2);

    std::list<int> sortedList3 = parallel_quick_sort(l);
    print(sortedList3);

    std::list<int> l1 = {0, 1, 2, 3, 4};
    std::list<int> l2;
    l2.splice(l2.begin(), l1);
    std::cout << "l1: ";
    print(l1);
    std::cout << "l2: ";
    print(l2);
    std::cout << "\n";

    l1 = {0, 1, 2, 3, 4};
    l2.clear();
    l2.splice(l2.begin(), l1, l1.begin());
    std::cout << "l1: ";
    print(l1);
    std::cout << "l2: ";
    print(l2);

    return 0;
}
