cmake_minimum_required(VERSION 3.21)
project(secondTask)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(secondTask
        bfs.h
        main.cpp)
