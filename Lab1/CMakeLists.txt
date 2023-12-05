cmake_minimum_required(VERSION 3.10)

project(lab1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-std=c++11 -O2")

add_executable(lab1 lab1.cpp)