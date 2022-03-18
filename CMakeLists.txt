cmake_minimum_required(VERSION 3.13)  # CMake version check
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard
project(6_FinanceOverseer)

set(6_FinanceOverseer "init_bank.cpp")

add_executable(6_FinanceOverseer 
  src/01_init_bank/init_bank.cpp
  src/02_process_transaction/process_transaction.cpp
)

set_target_properties(6_FinanceOverseer
    PROPERTIES 
        OUTPUT_NAME "initialize_bank")

target_include_directories(6_FinanceOverseer PRIVATE include)

set(MSYSPath "C:/msys64/mingw64")

include_directories(${MSYSPath}/include)

target_link_libraries(6_FinanceOverseer 
${MSYSPath}/bin/libxlnt.dll)



