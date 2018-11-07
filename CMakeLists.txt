cmake_minimum_required(VERSION 3.12)
project(CISC_361_Programming_Assignment_05 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(test00
        tests/test00.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(test01-thread-creation
        tests/test01-thread-creation.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(test01x-multi-thread-creation
        tests/test01x-multi-thread-creation.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(swap-context-yield
        ../../../.CLion2018.2/config/scratches/swap-context-yield.c)

add_executable(swap-context
        ../../../.CLion2018.2/config/scratches/swap-context.c)

add_executable(test00_original
        tests/test00.c
        ud_thread.h
        ../../../.CLion2018.2/config/scratches/t_lib_original.h
        ../../../.CLion2018.2/config/scratches/t_lib_original.c)

add_executable(test01a-multi-thread-creation
        tests/test01a-multi-thread-creation.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(test02-time-slicing
        tests/test02-time-slicing.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(test04-time-slicing
        tests/test04-time-slicing.c
        ud_thread.h
        t_lib.h
        t_lib.c)

add_executable(test07-testing-2lq
        tests/test07-testing-2lq.c
        ud_thread.h
        t_lib.h
        t_lib.c)