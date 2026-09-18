cmake_minimum_required(VERSION 3.14)
project(pt)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(pt)
add_subdirectory(src)
add_subdirectory(external)
