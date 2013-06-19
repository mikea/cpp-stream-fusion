cmake_minimum_required(VERSION 2.8)
project(stream)

add_definitions(-std=c++11)

INCLUDE(ExternalProject)
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_BASE ${CMAKE_BINARY_DIR}/third_party)

include(gtest.cmake)
include(boost.cmake)

include_directories(${GTEST_INCLUDE_DIR} ${BOOST_INCLUDE_DIR})

add_library(vector vector.cc)
target_link_libraries(vector pthread)
enable_testing()

add_executable(vector_test vector_test.cc)
target_link_libraries(vector_test gtest vector)
add_test(vector_test vector_test)
