//
// Created by 吴凡 on 2017/2/22.
//
#include <iostream>
#include <list>

/*
 *
 * namespace std {
 * class mutex {
 * public:
 *      mutex();
 *      mutex& mutex(const mutex&) = delete;
 * };
 * }
 */

std::list<int> lists;
std::mutex mutex;

void AddValue(int value) {
    std::lock_guard<std::mutex> lock(mutex);
    lists.push_back(value);
}

bool ContainsValue(int value) {
    std::lock_guard<std::mutex> lock(mutex);
    return std::find(lists.begin(), lists.end(), value) != lists.end();
}

int main() {
    std::cout << "Hello Mutex\n";
}