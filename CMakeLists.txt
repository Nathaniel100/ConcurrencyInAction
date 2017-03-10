cmake_minimum_required(VERSION 3.6)
project(ConcurrencyInAction)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS thread system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(chapter1_2)

add_subdirectory(chapter3)

add_subdirectory(chapter4)

add_subdirectory(chapter5)

add_subdirectory(chapter6)

