# set tracy options before add_subdirectory
option(TRACY_ENABLE "" ON) 
add_subdirectory(tracy) # target: TracyClient or alias Tracy::TracyClient