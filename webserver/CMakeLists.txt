cmake_minimum_required(VERSION 2.8.2)

project(webserver)

add_compile_options(-std=c++11)   # 添加编译选项  


aux_source_directory(. ALL_SRCS)  # 添加当前目录下所有源文件

include_directories(./threadpool ./locker)  # 添加头文件路径

# link_directories()     # 添加库的搜索路径

add_subdirectory(./http)   # 向当前工程中添加存放源文件的子目录  子目录中需要有一个CMakeLists.txt

set(CMAKE_BUILD_TYPE Debug)

add_executable(webserver ${ALL_SRCS})   # 生成可执行文件

target_link_libraries(webserver http)  # 添加链接库

target_link_libraries(webserver pthread)





 

