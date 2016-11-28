cmake_minimum_required(VERSION 2.8)

project(ArchLib)

# cpp sources
set(CPP_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/egos.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
)

set(C_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/hooks.c
)

set(ARCH_SRCS ${C_SRCS} ${CPP_SRCS})

# local includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11 -fexceptions -fmessage-length=0")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -fno-builtin -ffunction-sections -fdata-sections -frtti -funwind-tables")

if (NOT DEFINED TARGET_PLAT)
    add_executable(${CMAKE_PROJECT_NAME} ${ALL_SRCS} ${ARCH_SRCS})
else()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_PLAT})
endif()



