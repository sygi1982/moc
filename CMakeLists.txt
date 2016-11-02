cmake_minimum_required(VERSION 2.8)

project(SMARTCAN)

#platform
if (NOT DEFINED TARGET_ARCH)
    set(TARGET_ARCH "x86_64")
endif()

string(ASCII 27 Esc)
set(COLOR "${Esc}[33m")
set(END_COLOR "${Esc}[m")

message(STATUS "${COLOR}## Building for ${TARGET_ARCH}${END_COLOR}")

set(CMAKE_BINARY_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${TARGET_ARCH})
set(EXECUTABLE_OUTPUT_PATH
    ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH
   ${CMAKE_BINARY_DIR}/lib)

message(STATUS "${COLOR}## Binary output directory: ${EXECUTABLE_OUTPUT_PATH}${END_COLOR}")
message(STATUS "${COLOR}## Library output directory: ${LIBRARY_OUTPUT_PATH}${END_COLOR}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("${COLOR}-- ## Debug build${END_COLOR}")
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} -O0 -g -Wall -DDEBUG")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -O0 -g -Wall -DDEBUG")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message("${COLOR}-- ## Release build${END_COLOR}")
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} -O2 -Os -Wall -DRELEASE")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -O2 -Os -Wall -DRELEASE")
else()
    message(FATAL_ERROR "Wrong build type defined! Valid: Debug,Release")
endif()

# global includes
include_directories(${CMAKE_SOURCE_DIR}/inc)

# global sources
set(ALL_SCRS "")

# base component sources
add_subdirectory(${CMAKE_SOURCE_DIR}/base)
# core component sources
add_subdirectory(${CMAKE_SOURCE_DIR}/core)

# call platform specific cmake
add_subdirectory(${CMAKE_SOURCE_DIR}/arch/${TARGET_ARCH})
