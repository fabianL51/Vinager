cmake_minimum_required(VERSION 3.13)  # CMake version check
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard
# get variable project name
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

# add executable
add_executable(${ProjectId}
  src/process_monthly_overview.cpp
)

# executable name
set_target_properties(${ProjectId}
  PROPERTIES 
    OUTPUT_NAME "process_monthly_overview")


# get main Path
get_filename_component(SubModulePath ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(ModulePath ${SubModulePath} DIRECTORY)
get_filename_component(MainPath ${ModulePath} DIRECTORY)

# activate include in main Path
include_directories(${MainPath}/include)

# move executable to main Path
add_custom_command(TARGET ${ProjectId} 
  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${ProjectId}> ${MainPath}/exe)



