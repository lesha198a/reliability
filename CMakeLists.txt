cmake_minimum_required(VERSION 3.19)
project(program)

set(CMAKE_CXX_STANDARD 20)

add_executable(program main.cpp Reliable.cpp Reliable.h ReliableModed.h ReliableModed.cpp Redistribution.cpp Redistribution.h SchemeData.cpp SchemeData.h ReliableModed.cpp ReliableModed.h)