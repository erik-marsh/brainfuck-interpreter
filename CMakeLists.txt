cmake_minimum_required(VERSION 3.10)

project(brainfuck VERSION 0.1.0 LANGUAGES C)

add_compile_options(-std=c99 -Wall -Wpedantic -Wextra -Wundef -Wfloat-equal -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wconversion -Wunreachable-code -Wswitch-default -Wswitch-enum)

add_subdirectory(src)
add_subdirectory(lib)