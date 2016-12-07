cmake_minimum_required(VERSION 2.8)

project(SMARTCAN)

#platform
if (NOT DEFINED TARGET_OS)
    set(TARGET_OS "linux")
endif()

string(ASCII 27 Esc)
set(COLOR "${Esc}[33m")
set(END_COLOR "${Esc}[m")

message(STATUS "${COLOR}## Building for ${TARGET_OS}${END_COLOR}")

set(CMAKE_BINARY_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${TARGET_OS})
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
add_subdirectory(${CMAKE_SOURCE_DIR}/os/${TARGET_OS})

# unit tests using gtest
if (BUILD_UT)
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    set(TARGET_UT UnitTests)
    set(UT_FILES tests/osapi_ut.cpp)

    enable_testing()
    add_executable(${TARGET_UT} ${UT_FILES})
    target_link_libraries(${TARGET_UT} ${GTEST_LIBRARIES} pthread)
    set_target_properties(${TARGET_UT} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
    add_test(${TARGET_UT} ${TARGET_UT})
endif()


