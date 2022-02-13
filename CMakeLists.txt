cmake_minimum_required(VERSION 3.21)
project(CppPlayground)

set(CMAKE_CXX_STANDARD 20)

add_executable(CppPlayground main.cpp)
add_executable(InvokeTest InvokeTest.cpp)
add_executable(PlacementNewTest PlacementNewTest.cpp)