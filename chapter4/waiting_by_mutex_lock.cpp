//
// Created by 吴凡 on 2017/2/24.
//


#include <mutex>
#include <thread>

bool flag;
std::mutex mutex;

// If the sleeping time is appropriate, This way don't waste processing time while it's sleeping.
// But it is hard to get the right sleep period.
// So we can use Condition Variable
void wait_for_flag() {
    std::unique_lock<std::mutex> lock(mutex);
    while(!flag) {
        lock.unlock(); // unlock the mutex
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // sleep for 100ms
        lock.lock(); // relock the mutex
    }
    lock.unlock();
}

void set_flag() {
    std::lock_guard<std::mutex> lock(mutex);
    flag = true;
}

int main() {
    std::thread t1{wait_for_flag};
    std::thread t2{set_flag};
    t1.join();
    t2.join();
}
