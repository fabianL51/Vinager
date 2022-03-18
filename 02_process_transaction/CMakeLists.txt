cmake_minimum_required(VERSION 3.13)  # CMake version check
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard
# get variable project name
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

# add executable
add_executable(${ProjectId}
  src/process_transaction.cpp
)

# executable name
set_target_properties(${ProjectId}
    PROPERTIES 
        OUTPUT_NAME "process_transaction")


# get main Path
get_filename_component(MainPath ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

include_directories(${MainPath}/include)

target_link_libraries(${ProjectId} 
${MainPath}/lib/libxlnt.dll)



