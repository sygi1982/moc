cmake_minimum_required(VERSION 2.8)

project(ArchLib)

set(ARCH_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/egos.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -fexceptions -fmessage-length=0")
set(CMAKE_CXX_FLAGS
   "${CMAKE_CXX_FLAGS} -fno-builtin -ffunction-sections -fdata-sections -frtti -funwind-tables")

add_executable(${CMAKE_PROJECT_NAME} ${ALL_SRCS} ${ARCH_SRCS})
