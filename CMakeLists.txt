cmake_minimum_required(VERSION 3.6)
project(ConcurrencyInAction)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(ConcurrencyInAction ${SOURCE_FILES})

add_executable(HelloWorld hello.cpp)

add_executable(Constructor constructor.cpp)

add_executable(PassingArguments passing_arguments.cpp)

add_executable(Ownership ownership.cpp)

add_executable(ParallelAccumulate parallel_accumulate.cpp)

add_subdirectory(chapter3)

add_subdirectory(chapter4)

add_subdirectory(chapter5)

add_subdirectory(chapter6)

