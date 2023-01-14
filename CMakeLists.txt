cmake_minimum_required(VERSION 3.24)
project(program)

set(CMAKE_CXX_STANDARD 20)

add_executable(program main.cpp LogicMath.cpp LogicMath.h Reliable.cpp Reliable.h)