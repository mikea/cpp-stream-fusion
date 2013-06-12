cmake_minimum_required(VERSION 2.8)
project(stream)

add_definitions(-std=c++11)
add_executable(stream stream.cc)

enable_testing()
add_test(basic stream)
