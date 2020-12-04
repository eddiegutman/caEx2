cmake_minimum_required(VERSION 3.17)
project(caEx2 C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_executable(caEx2 src/ex2.c)