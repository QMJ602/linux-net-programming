cmake_minimum_required(VERSION 2.8.0)

project(UDP_CLIENT)

set(SRC src/main.cpp src/udp_client.cpp)

set(CMAKE_BUILD_TYPE Debug)

include_directories(./include)

add_executable(udp_client ${SRC})


