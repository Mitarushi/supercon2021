cmake_minimum_required(VERSION 3.19)
project(supercon2021)

set(CMAKE_CXX_STANDARD 20)

add_executable(supercon2021
        combined.cpp)

add_executable(sample_yesno test/sample_yesno.cpp)
add_executable(vertex-edge_count analysis/vertex-edge_count/vertex-edge_count.cpp)
add_executable(greedy_check test/greedy_check.cpp)