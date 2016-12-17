cmake_minimum_required(VERSION 2.8)

project(OsLib)

# cpp sources
set(CPP_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/egos.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
)

set(C_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/hooks.c
)

set(OS_SRCS ${C_SRCS} ${CPP_SRCS})

# local includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++1y -fexceptions -fmessage-length=0")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -fno-builtin -ffunction-sections -fdata-sections -frtti -funwind-tables")

if (NOT DEFINED TARGET_PLAT)
    set(DEFAULT_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/default/hal.cpp
    )

    add_executable(${CMAKE_PROJECT_NAME} ${ALL_SRCS} ${OS_SRCS} ${DEFAULT_SRCS})
else()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_PLAT})
endif()



