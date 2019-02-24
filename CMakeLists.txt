cmake_minimum_required(VERSION 3.13)
project(ImageConversion C)

set(CMAKE_C_STANDARD 99)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(ImageConversion main.c BitmapReader.c BitmapReader.h benchmark.c benchmark.h)