cmake_minimum_required(VERSION 3.13)

project(task6 LANGUAGES CXX)

add_subdirectory(src)

find_package(OpenMP REQUIRED)
target_link_libraries(task6 PUBLIC OpenMP::OpenMP_CXX)