cmake_minimum_required(VERSION 3.15)
project(lab2)

find_package(Boost)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
add_executable(lab2 main.cpp)