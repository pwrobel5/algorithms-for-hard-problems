cmake_minimum_required(VERSION 3.16)
project(algorithms_for_hard_problems)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g")

add_executable(algorithms_for_hard_problems main.cpp src/Graph.cpp src/Graph.h src/Vertex.cpp src/Vertex.h src/Solver.cpp src/Solver.h src/NoSolutionException.h)