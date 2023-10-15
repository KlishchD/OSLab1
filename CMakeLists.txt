cmake_minimum_required(VERSION 3.23)
project(OSLab1)

set(CMAKE_CXX_STANDARD 17)

add_executable(functions functions.cpp Result.cpp Result.h Utils.cpp Utils.h StatusCodes.h)
add_executable(app app.cpp Result.cpp Result.h Utils.h Utils.cpp Manager.cpp Manager.h StatusCodes.h)
