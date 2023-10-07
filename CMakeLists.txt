cmake_minimum_required(VERSION 3.23)
project(OSLab1)

set(CMAKE_CXX_STANDARD 14)

add_executable(functions functions.cpp Result.cpp Result.h)
add_executable(manager manager.cpp Result.cpp Result.h)
