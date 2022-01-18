cmake_minimum_required(VERSION 3.20)
project(gol)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP)
include(FindOpenMP)

if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(gol main.cpp gol.cpp gol.h Timing.cpp Timing.h)
