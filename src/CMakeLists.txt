file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.h"
    )

#add_library (src STATIC 
#  "${SOURCES}"
#)

add_executable(raytracing-1 main.cpp 
	${SOURCES}
	"${PROJECT_SOURCE_DIR}/thirdparty/tracy/public/TracyClient.cpp"
)

target_include_directories(raytracing-1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(raytracing-1 PUBLIC Tracy::TracyClient)

set_property(TARGET raytracing-1 PROPERTY CXX_STANDARD 23)