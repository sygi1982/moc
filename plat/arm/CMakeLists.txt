cmake_minimum_required(VERSION 2.8)

project(PlatLib)

if (NOT EXISTS ${LIBRARY_OUTPUT_PATH})
    file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})
endif()

add_custom_target(
    platlib
    COMMAND make CC=${CMAKE_C_COMPILER} CFLAGS=${CMAKE_C_FLAGS} OBJDIR=${CMAKE_CURRENT_BINARY_DIR} OUTDIR=${LIBRARY_OUTPUT_PATH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PLAT_SRCS
    os.cpp
)

add_executable(TEST ${ALL_SRCS} ${PLAT_SRCS})
add_dependencies(TEST platlib)
target_link_libraries(TEST ${LIBRARY_OUTPUT_PATH}/platlib.a)

