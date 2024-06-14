cmake_minimum_required(VERSION 3.13)

project(task5 LANGUAGES CXX)

find_package(MKL CONFIG REQUIRED PATHS $ENV{MKLROOT})
find_package(MPI REQUIRED) # 添加这一行来找到MPI库

add_executable(matmul matmul_thread_omp.cpp)
target_link_libraries(matmul PUBLIC MKL::MKL)
find_package(OpenMP REQUIRED)
target_link_libraries(matmul PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(matmul PUBLIC MPI::MPI_CXX) # 添加这一行来链接MPI库