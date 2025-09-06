cmake_minimum_required(VERSION 3.15...4.0)

project("raytracing-1")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_subdirectory(src)
add_subdirectory(thirdparty)
