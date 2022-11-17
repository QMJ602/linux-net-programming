cmake_minimum_required(VERSION 2.8.2)

project(SERVER)

set(SRC http_conn.cpp main.cpp)

set(CMAKE_BUILD_TYPE Debug)

add_definitions(-std=c++11)

add_executable(server ${SRC})