cmake_minimum_required(VERSION 3.20)
project(6502lib LANGUAGES C)
project(6502test LANGUAGES C)

set(CMAKE_BUILD_TYPE Debug)

set(INCLUDE_DIR include/)
set(SOURCES src/cpu.c src/args.c src/dbg_parse.c)

add_library(6502lib ${SOURCES})
target_include_directories(6502lib PRIVATE include/)

add_executable(6502test src/main.c ${SOURCES})
target_include_directories(6502test PRIVATE include/)

install(TARGETS 6502lib DESTINATION /usr/local/lib)
install(TARGETS 6502test DESTINATION /usr/local/bin)
