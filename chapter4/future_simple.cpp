//
// Created by 吴凡 on 2017/2/24.
//

#include <future>
#include <iostream>

int find_the_answer_to_ltuae() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 42;
}
void do_other_stuff() {
    std::cout << "Do other stuff\n";
}

int main() {
    std::future<int> future = std::async(find_the_answer_to_ltuae);
    do_other_stuff();
    std::cout << "The answer is " << future.get() << "\n";
}

