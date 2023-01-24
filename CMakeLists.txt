cmake_minimum_required(VERSION 3.19)
project(program)

set(CMAKE_CXX_STANDARD 20)

add_executable(program main.cpp Reliable.cpp Reliable.h Redistribution.cpp Redistribution.h CircuitModel.cpp CircuitModel.h)