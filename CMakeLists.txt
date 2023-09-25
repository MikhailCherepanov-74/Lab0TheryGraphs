cmake_minimum_required(VERSION 3.24)
project(Ther_graph_Lab0)

set(CMAKE_CXX_STANDARD 17)

add_executable(Ther_graph_Lab0 main.cpp Tree.h)
include_directories(/usr/lib/x86_64-linux-gnu/mpich/include/)
target_link_libraries(Ther_graph_Lab0 ${MPI_CXX_LIBRARIES})
set(CMAKE_C_COMPILER «/usr/bin/mpicc»)
set(MPI_EXEC_NUM_THREADS 3)