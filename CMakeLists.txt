cmake_minimum_required(VERSION 3.20)
project(6502lib LANGUAGES C)

set(SOURCES src/cpu.c)
add_library(6502lib ${SOURCES})
target_include_directories(6502lib PRIVATE include/)
