add_subdirectory(glm)

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)
add_subdirectory(tracy)

target_link_libraries(pt PRIVATE
  glm
  TracyClient
)

target_include_directories(pt PRIVATE
  glm
  stb
  tracy
)
