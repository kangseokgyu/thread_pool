cmake_minimum_required(VERSION 3.13.0)
project(thread_pool CXX)
set (CMAKE_CXX_STANDARD 14)

include(CTest)
add_subdirectory(example)

add_library(thread_pool INTERFACE)
