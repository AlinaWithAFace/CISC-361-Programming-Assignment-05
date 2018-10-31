cmake_minimum_required(VERSION 3.12)
project(CISC_361_Programming_Assignment_05 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(test00
        test00.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(thread-creation-test
        thread-creation-test.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(multi-thread-creation-test
        multi-thread-creation-test.c
        ud_thread.h
        t_lib.h
        t_lib.c)
