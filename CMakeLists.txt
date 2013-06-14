cmake_minimum_required(VERSION 2.8)
project(stream)

include(gtest.cmake)
include_directories(${GTEST_INCLUDE_DIR})

add_definitions(-std=c++11)
# add_definitions(-stdlib=libc++)
# add_definitions(-lc++abi)
add_library(vector vector.cc)
enable_testing()


add_executable(vector_test vector_test.cc)
add_dependencies(vector_test gtest) # TODO: this seem to have no effect
target_link_libraries(vector_test gtest)

add_test(vector_test vector_test)
