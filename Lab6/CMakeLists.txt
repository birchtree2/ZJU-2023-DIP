cmake_minimum_required(VERSION 3.10)

project(lab5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-std=c++11 -O2")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}) 
add_executable(lab6 main.cpp bmplib.cpp bilateral.cpp)