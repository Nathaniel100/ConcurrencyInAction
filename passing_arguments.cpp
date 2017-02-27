//
// Created by 吴凡 on 2017/2/21.
//

#include <thread>
#include <iostream>


void f(int i, const std::string &s) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << i << "--" << s << '\n';
}

void oops() {
    int i = 100;
    char buffer[128] = {0};
    snprintf(buffer, 128, "%d", i);
    std::thread t{f, i, buffer}; // !! Converting to string is in thread context that leads to undefined behavior
    // std::thread t{f, i, std::string(buffer)}; // use this
    t.detach();
}


/*
void update_widget(widget_id id, wdiget_data &data);
void oops_again(widget_id id) {
    widget_data data;
    std::thread t{update_widget, id, data}; // not useful. see constructor
    // std::thread t{update_widget, id, std::ref(data)}; // use this
    t.join();
    process_widget_data(data);
}
*/

struct Foo {
    void FooFunc() {
        std::cout << "Foo Function\n";
    }
};

void f() {
    Foo f;
    std::thread t{&Foo::FooFunc, &f};
    t.join();
}

struct BigObject {

};
void ProcessBigObject(std::unique_ptr<BigObject> ptr) {
    // do something
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "ProcessBigObject\n";
}

void f2() {
    std::unique_ptr<BigObject> ptr(new BigObject);
    // BigObject ownership first into thread internal storage, then into ProcessBigObject
    std::thread  t{ProcessBigObject, std::move(ptr)};
    t.join();
}

int main() {
    std::thread t{[](int x) {
        std::cout << "square(" << x << "): " << x * x << '\n';
    }, 2};
    t.join();

    //oops();
    f();
    f2();
    getchar();
}

