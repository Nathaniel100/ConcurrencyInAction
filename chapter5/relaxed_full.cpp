//
// Created by 吴凡 on 2017/3/3.
//



#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> x, y, z;
const int loop_count = 10;
struct Val{
    int x, y, z;
};
Val values1[loop_count];
Val values2[loop_count];
Val values3[loop_count];
Val values4[loop_count];
Val values5[loop_count];
std::atomic<int> go;

void increment(std::atomic<int> *var_to_inc, Val* values) {
    while(!go) std::this_thread::yield();
    for(int i = 0; i < loop_count; ++i) {
        values[i].x = x.load(std::memory_order_relaxed);
        values[i].y = y.load(std::memory_order_relaxed);
        values[i].z = z.load(std::memory_order_relaxed);
        var_to_inc->store(i+1, std::memory_order_relaxed);
        std::this_thread::yield();
    }
}

void read_vals(Val* values) {
    while(!go) std::this_thread::yield();
    for(int i = 0; i < loop_count; ++i) {
        values[i].x = x.load(std::memory_order_relaxed);
        values[i].y = y.load(std::memory_order_relaxed);
        values[i].z = z.load(std::memory_order_relaxed);
        std::this_thread::yield();
    }
}

void print(Val* values) {
    for(int i = 0; i < loop_count; ++i) {
        if(i) std::cout << ",";
        std::cout << "(" << values[i].x << "," << values[i].y << "," << values[i].z << ")";
    }
    std::cout << "\n";
}

int main() {
    x = 0;
    y = 0;
    z = 0;
    go = false;
    std::thread t1{increment, &x, values1};
    std::thread t2{increment, &y, values2};
    std::thread t3{increment, &z, values3};
    std::thread t4{read_vals, values4};
    std::thread t5{read_vals, values5};

    go = true;

    t5.join();
    t4.join();
    t3.join();
    t2.join();
    t1.join();

    print(values1);
    print(values2);
    print(values3);
    print(values4);
    print(values5);

}


