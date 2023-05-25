cmake_minimum_required(VERSION 3.17.5)
project(os)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_COMPILER "gcc")

add_executable(os fileSystem.h test.cpp)