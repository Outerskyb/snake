cmake_minimum_required(VERSION 3.10)
project(snake C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(snake main.c)

if(MSVC)
    target_compile_options(snake PRIVATE /W4)
    target_compile_definitions(snake PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
