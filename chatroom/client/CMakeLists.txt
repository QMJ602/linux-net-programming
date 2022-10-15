cmake_minimum_required(VERSION 2.8.2)

project(CLIENT)

set(SRC src/client.cpp)

set(CMAKE_BUILD_TYPE Debug)

include_directories(./include)

add_executable(client ${SRC})

