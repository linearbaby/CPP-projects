cmake_minimum_required(VERSION 3.10)
project(IZ1_release)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runTests test.cpp list_functions.c ship_functions.c)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

add_executable(IZ1_release ship_header.h mylist.h main.c list_functions.c ship_functions.c)