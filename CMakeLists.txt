cmake_minimum_required(VERSION 2.8)
project(stream)

add_definitions(-std=c++11)
add_library(vector vector.cc)

enable_testing()
add_executable(vector_test vector_test.cc)
add_test(vector_test vector_test)
