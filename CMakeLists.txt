# cmake_minimum_required(VERSION <specify CMake version here>)
project(files C)

set(CMAKE_C_STANDARD 11)

add_executable(files main.c os_types.h data_types.h)