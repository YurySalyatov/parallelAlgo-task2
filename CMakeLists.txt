cmake_minimum_required(VERSION 3.21)
project(parallelAlgo2)

set(CMAKE_CXX_STANDARD 14)

find_package(TBB CONFIG REQUIRED)

include_directories(parallelAlgo-task2)

add_executable(parallelAlgo2
    parallelAlgo-task2/bfs.h
    parallelAlgo-task2/main.cpp
    parallelAlgo-task2/graph.h)

target_link_libraries(parallelAlgo2 TBB::tbb)