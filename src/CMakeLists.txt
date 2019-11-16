cmake_minimum_required(VERSION 3.14)
project(xcw)

set(CMAKE_CXX_STANDARD 14)

add_executable(xcw main.cpp mytest.cpp mytest.h test_alloc.cpp local.h)