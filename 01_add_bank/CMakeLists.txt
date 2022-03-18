cmake_minimum_required(VERSION 3.13)  # CMake version check
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard
project(9_Vinager)

set(6_FinanceOverseer "init_bank.cpp")

add_executable(9_Vinager 
  src/add_bank.cpp
)

set_target_properties(9_Vinager
    PROPERTIES 
        OUTPUT_NAME "initialize_bank")


set(MSYSPath "C:/msys64/mingw64")
set(MainPath "D:/Project/9_Vinager")

include_directories(${MSYSPath}/include)
include_directories(${MainPath}/include)

target_link_libraries(9_Vinager 
${MSYSPath}/bin/libxlnt.dll)



