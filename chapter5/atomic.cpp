//
// Created by 吴凡 on 2017/3/3.
//

#include <atomic>

/*
 *
 * namespace std {
 *
 * template<class T> class atomic;
 *
 * template<> class atomic<bool>;
 *
 * template<> class atomic<Integral>;
 *
 * template<class T> class atomic<T*>;
 *
 * template<class T>
 * class atomic {
 * public:
 *      atomic() = default;
 *      atomic(T desired);
 *      atomic(const atomic&) = delete;
 *      T operator=(T desired);
 *      atomic& operator=(const atomic&) = delete;
 *
 *      bool is_lock_free();
 *      T load(std::memory_order);
 *      void store(T desired, std::memory_order);
 *      T exchange(T desired, std::memory_order);
 *      bool compare_exchange_weak(T& expected, T desired, std::memory_order success, std::memory_order failure);
 *      bool compare_exchange_strong(T& expected, T desired, std::memory_order success, std::memory_order failure);
 *
 *      T fetch_add(T, std::memory_order);
 *      ....
 *
 * };
 *
 * }
 *
 *
 *
 */

