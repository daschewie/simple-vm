cmake_minimum_required(VERSION 3.10)
project(untitled C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled vm.c vm.h main.c assembler.c)