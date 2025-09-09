# set tracy options before add_subdirectory
option(TRACY_ENABLE "" ON) 
option(TRACY_ON_DEMAND "" ON)
add_subdirectory(tracy) # target: TracyClient or alias Tracy::TracyClient