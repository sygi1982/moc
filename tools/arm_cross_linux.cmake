set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_SYSTEM_VERSION 1)

#set(CMAKE_ASM_COMPILER_WORKS 1)
#set(CMAKE_C_COMPILER_WORKS   1)
#set(CMAKE_CXX_COMPILER_WORKS 1)
# specifiy target cpu flags
set(PLAT_CONFIG_FLAGS "-D_GLIBCXX_USE_NANOSLEEP")
# combine flags to C and C++ flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PLAT_CONFIG_FLAGS}" CACHE STRING "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PLAT_CONFIG_FLAGS}" CACHE STRING "")
# target environment
set(TOOLCHAIN_PREFIX arm-linux-gnueabi-)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(TOOLCHAIN_ROOT /usr/arm-linux-gnueabi/)
set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_ROOT})
# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)