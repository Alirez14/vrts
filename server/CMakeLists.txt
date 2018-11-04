cmake_minimum_required(VERSION 3.12)
project(server)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(server
        functions.h
        main.cpp)
