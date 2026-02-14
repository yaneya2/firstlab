cmake_minimum_required(VERSION 4.0)
project(firstlab C)

set(CMAKE_C_STANDARD 17)

add_executable(firstlab main.c
        headers/List.h
        c/List.c
        c/FieldInfo.c
        headers/FieldInfo.h
        headers/Allocator.h)
