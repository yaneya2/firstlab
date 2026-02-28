cmake_minimum_required(VERSION 4.0)
project(firstlab C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
add_executable(firstlab main.c
        headers/DynamicArray.h
        c/DynamicArray.c
        c/FieldInfo.c
        headers/FieldInfo.h
        headers/Allocator.h
        headers/UsersStruct.h)
enable_testing()
add_executable(tests tests.c
        headers/DynamicArray.h
        c/DynamicArray.c
        c/FieldInfo.c
        headers/FieldInfo.h
        headers/Allocator.h
)
add_test(NAME UnitTests COMMAND tests)