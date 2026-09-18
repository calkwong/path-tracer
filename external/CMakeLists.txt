add_subdirectory(glm)

target_link_libraries(pt PRIVATE
  glm
)

target_include_directories(pt PRIVATE
  glm
  stb
)
