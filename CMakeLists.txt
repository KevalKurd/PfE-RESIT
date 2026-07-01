cmake_minimum_required(VERSION 3.21)

project(CW C)

set(CMAKE_C_STANDARD 99)

add_executable(CW
        main.c
        adc.c
        io.c
        stats.c
        )