cmake_minimum_required(VERSION 3.1)
project(Graphics CXX)

find_package(OpenMP)

set (CMAKE_CXX_FLAGS "-std=c++17 -O3 ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -mtune=native -march=native")

add_executable(main main.cpp)