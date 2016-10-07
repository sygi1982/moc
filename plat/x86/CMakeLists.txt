cmake_minimum_required(VERSION 2.8)

set(PLAT_SRCS
    os.cpp
)

add_executable(TEST ${ALL_SRCS} ${PLAT_SRCS})
