file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.h"
    )

#add_library (src STATIC 
#  "${SOURCES}"
#)

add_executable(raytracing-1 main.cpp ${SOURCES})

target_include_directories(raytracing-1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set_property(TARGET raytracing-1 PROPERTY CXX_STANDARD 23)