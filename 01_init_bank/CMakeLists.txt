cmake_minimum_required(VERSION 3.13)  # CMake version check
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard
project(6_FinanceOverseer)

set(6_FinanceOverseer "init_bank.cpp")

add_executable(6_FinanceOverseer 
  src/init_bank.cpp
)

set_target_properties(6_FinanceOverseer
    PROPERTIES 
        OUTPUT_NAME "initialize_bank")


set(MSYSPath "C:/msys64/mingw64")
set(MainPath "D:/Project/6_FinanceOverseer")

include_directories(${MSYSPath}/include)
include_directories(${MainPath}/include)

target_link_libraries(6_FinanceOverseer 
${MSYSPath}/bin/libxlnt.dll)



