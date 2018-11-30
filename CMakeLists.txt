cmake_minimum_required(VERSION 3.12)
project(CISC_361_Programming_Assignment_05 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(hello-threads
        tests/test00.c
        src/ud_thread.h
        src/t_lib.h
        src/t_lib.c)

add_executable(thread-creation
        tests/test01-thread-creation.c
        src/ud_thread.h
        src/t_lib.h
        src/t_lib.c)

add_executable(multi-thread-creation-a
        tests/test01x-multi-thread-creation.c
        src/ud_thread.h
        src/t_lib.h
        src/t_lib.c)

#add_executable(swap-context-yield
#        ../../../.CLion2018.2/config/scratches/swap-context-yield.c)

#add_executable(swap-context
#        ../../../.CLion2018.2/config/scratches/swap-context.c)

#add_executable(test00_original
#        tests/test00.c
#        src/ud_thread.h
#        ../../../.CLion2018.2/config/scratches/t_lib_original.h
#        ../../../.CLion2018.2/config/scratches/t_lib_original.c)

add_executable(multi-thread-creation-b
        tests/test01a-multi-thread-creation.c
        src/ud_thread.h
        src/t_lib.h
        src/t_lib.c)

add_executable(time-slicing-a
        tests/test02-time-slicing.c
        src/ud_thread.h
        src/t_lib.h
        src/t_lib.c)

add_executable(time-slicing-b
        tests/test04-time-slicing.c
        src/ud_thread.h
        src/t_lib.h
        src/t_lib.c)

add_executable(two-level-queue
        tests/test07-testing-2lq.c
        src/ud_thread.h
        src/t_lib.h
        src/t_lib.c)

add_executable(alarm
        samples/test_alarm.c)

add_executable(send-recieve
        tests/test05-send-recieve.c
        src/ud_thread.h
        src/t_lib.h
        src/t_lib.c)

add_executable(send-recieve-asynchronous
        tests/test08-asynchronous-send-recieve.c
        src/ud_thread.h
        src/t_lib.h
        src/t_lib.c)

add_executable(mailbox
        tests/test06-mailbox.c
        src/ud_thread.h
        src/t_lib.h
        src/t_lib.c)
