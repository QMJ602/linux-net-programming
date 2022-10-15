cmake_minimum_required(VERSION 2.8.2)

project(SERVER)

set(SRC src/server.cpp)

set(CMAKE_BUILD_TYPE Debug)

add_definitions(-std=c++11)

include_directories(./include)

add_executable(server ${SRC})