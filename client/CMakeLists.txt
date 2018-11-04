cmake_minimum_required(VERSION 3.12)
project(client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(client
        header.h
        main.cpp)
