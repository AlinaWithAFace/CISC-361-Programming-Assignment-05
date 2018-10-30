cmake_minimum_required(VERSION 3.12)
project(CISC_361_Programming_Assignment_05 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(CISC_361_Programming_Assignment_05
        t_lib.c
        t_lib.h
        test00.c
        ud_thread.h)
