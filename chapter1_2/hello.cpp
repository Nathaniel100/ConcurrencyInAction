//
// Created by 吴凡 on 2017/2/21.
//


#include <thread>
#include <iostream>

void hello() {
    std::cout << "Hello Concurrent World\n";
}

int main() {
    std::thread t{hello};
    t.join();
    return 0;
}
