cmake_minimum_required(VERSION 3.8)
project(FindString)

set(CMAKE_C_FLAGS "-Wextra -Wall -Wvla -std=c99")

set(SOURCE_FILES FindString.c)
add_executable(FindString ${SOURCE_FILES})