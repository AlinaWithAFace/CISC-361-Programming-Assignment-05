cmake_minimum_required(VERSION 3.12)
project(CISC_361_Programming_Assignment_05 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(test00
        ../../../.CLion2018.2/config/scratches/test00.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(thread-creation-test
        ../../../.CLion2018.2/config/scratches/thread-creation-test.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(multi-thread-creation-test
        ../../../.CLion2018.2/config/scratches/multi-thread-creation-test.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(swap-context-yield
        ../../../.CLion2018.2/config/scratches/swap-context-yield.c)

add_executable(swap-context
        ../../../.CLion2018.2/config/scratches/swap-context.c)

add_executable(test00_original
        ../../../.CLion2018.2/config/scratches/test00.c
        ud_thread.h
        ../../../.CLion2018.2/config/scratches/t_lib_original.h
        ../../../.CLion2018.2/config/scratches/t_lib_original.c)