find_package(TBB REQUIRED)
if (TBB_FOUND)
  target_link_libraries(pt PRIVATE TBB::tbb)
endif()

target_sources(pt PRIVATE
  main.cpp
  extern.cpp
  ray.cpp
  hittable.cpp
  world.cpp
  interval.cpp
  camera.cpp
  rando.cpp
  )

set_target_properties(pt PROPERTIES CXX_STANDARD 20)
