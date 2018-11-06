cmake_minimum_required(VERSION 3.12)
project(server2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(server2
        functions.h
        main.cpp)

target_link_libraries(server2 -lldap -llber)