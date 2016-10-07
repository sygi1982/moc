cmake_minimum_required(VERSION 2.8)

project(TEST)

#platform
if (NOT DEFINED TARGET_PLAT)
    set(TARGET_PLAT "x86")
endif()

message(STATUS "Building for ${TARGET_PLAT}")

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${TARGET_PLAT})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

message(STATUS "Binary output directory: ${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "Library output directory: ${LIBRARY_OUTPUT_PATH}")

# global includes
include_directories(${CMAKE_SOURCE_DIR}/include)

# global sources
set(ALL_SCRS "")

# base component sources
add_subdirectory(${CMAKE_SOURCE_DIR}/base)
# core component sources
add_subdirectory(${CMAKE_SOURCE_DIR}/core)

# call platform specific cmake
add_subdirectory(${CMAKE_SOURCE_DIR}/plat/${TARGET_PLAT})
