//
// Created by 吴凡 on 2017/2/27.
//


#include <list>
#include <iostream>

template<class T> class Type;


template<class ForwardIter>
ForwardIter partition(ForwardIter begin, ForwardIter end) {
    auto& pivot = *begin;
    ForwardIter i = begin;
    ForwardIter j = begin;
    std::advance(j, 1);
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
    ForwardIter q = p;
    std::advance(q, 1);
    sqs(q, end);
}

template<class T>
std::list<T> sequential_quick_sort(std::list<T> list) {
    if(list.empty()) return list;
    sqs(list.begin(), list.end());
    return list;
}

int main() {
    std::list<int> l = {5, 1, 9, 8, 4, 7, 3, 2, 0, 6};
    std::list<int> sortL = sequential_quick_sort(l);
    std::for_each(sortL.begin(), sortL.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "\n";
    return 0;
}
