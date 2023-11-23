cmake_minimum_required(VERSION 3.12)
project(wheel)

set(CMAKE_CXX_STANDARD 17)

# Create an executable target
add_executable(ts_01_client sandbox/tiny_socket/cpp_communicate_cpp/client.cpp)
add_executable(ts_01_server sandbox/tiny_socket/cpp_communicate_cpp/server.cpp)
add_executable(ts_02_server sandbox/tiny_socket/cpp_communicate_python/client.cpp)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
message("${CMAKE_CURRENT_LIST_DIR}/include/")