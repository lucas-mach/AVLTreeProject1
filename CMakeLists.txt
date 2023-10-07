cmake_minimum_required(VERSION 3.26)
project(Project1)

set(CMAKE_CXX_STANDARD 14)

include_directories(test-unit)

add_executable(Project1
        src/AVLTree.cpp
        src/main.cpp
        test-unit/catch.hpp
        test-unit/test.cpp)
