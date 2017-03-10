

add_executable(ThreadSafeQueue2 threadsafe_queue2.cpp)

add_executable(Queue queue.cpp)

add_executable(Queue2 queue2.cpp)

add_executable(ThreadSafeQueue3 threadsafe_queue3.cpp)

add_executable(ThreadSafeQueue4 threadsafe_queue4.cpp)

add_executable(LookupTable lookup_table.cpp)

target_link_libraries(LookupTable ${Boost_LIBRARIES})
