cmake_minimum_required(VERSION 2.8)

project(PlatLib)

set(C_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/syscalls.c
	#    ${CMAKE_CURRENT_SOURCE_DIR}/target.c
    ${CMAKE_CURRENT_SOURCE_DIR}/system_LPC17xx.c
	#    ${CMAKE_CURRENT_SOURCE_DIR}/ft245.c
)

set(CPP_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp_hooks.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/startup_lpc176x.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dbg_channel.cpp
)

set(PLAT_SRCS ${C_SRCS} ${CPP_SRCS})

# local includes
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

# CMSIS includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Drivers/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Core/Device/NXP/LPC17xx/Include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Core/CMSIS/Include)

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -mcpu=cortex-m3 -mthumb")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -mcpu=cortex-m3 -mthumb -DCPP_HEAP_EXCEPTIONS")
set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -nostdlib -Xlinker --Map=${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.map -Xlinker --gc-sections")
set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_CURRENT_SOURCE_DIR}/ld/lpc1764.ld -L${CMAKE_CURRENT_SOURCE_DIR}/ld")
set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -L${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(EXE_NAME
    ${CMAKE_PROJECT_NAME}.elf)

add_executable(${EXE_NAME} ${ALL_SRCS} ${OS_SRCS} ${PLAT_SRCS})

set(CMSIS_NAME
    DriversLPC17xxgnu.a)

if (BUILD_CMSIS)
    add_custom_target(
        cmsis
#        COMMAND make CC=${CMAKE_C_COMPILER} CFLAGS=${CMAKE_C_FLAGS} OBJDIR=${CMAKE_CURRENT_BINARY_DIR} OUTDIR=${LIBRARY_OUTPUT_PATH}
        COMMAND make TOOLCHAIN_PATH=${TOOLCHAIN_ROOT} TOOLCHAIN_VER=${TOOLCHAIN_VERSION}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmsis
    )

    add_dependencies(${EXE_NAME} cmsis)
    target_link_libraries(${EXE_NAME}
                          ${CMAKE_CURRENT_SOURCE_DIR}/cmsis/Drivers/library/${CMSIS_NAME})
else()
    target_link_libraries(${EXE_NAME}
                          ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMSIS_NAME})
endif()

target_link_libraries(${EXE_NAME}
                      ${CMAKE_CURRENT_SOURCE_DIR}/lib/libc_newlib.a)
target_link_libraries(${EXE_NAME}
                      ${CMAKE_CURRENT_SOURCE_DIR}/lib/libg_newlib.a)
target_link_libraries(${EXE_NAME}
                      ${CMAKE_CURRENT_SOURCE_DIR}/lib/libm_newlib.a)

# create flash files during post-build
add_custom_command(
    OUTPUT ${EXE_NAME}.bin
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    COMMAND ${TOOLCHAIN_PREFIX}objcopy -O binary ${EXE_NAME} ${EXE_NAME}.bin
    COMMENT "Creating binary flash image !"
)

add_custom_command(
    OUTPUT ${EXE_NAME}.hex
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    COMMAND ${TOOLCHAIN_PREFIX}objcopy -O ihex ${EXE_NAME} ${EXE_NAME}.hex
    COMMENT "Creating hex flash image !"
)

add_custom_target(flash_images ALL DEPENDS ${EXE_NAME}.bin ${EXE_NAME}.hex
    COMMAND ${TOOLCHAIN_PREFIX}size ${EXE_NAME}
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

set(DEPLOY_TOOL ${CMAKE_SOURCE_DIR}/tools/lpc21isp)

add_custom_target(deploy DEPENDS ${EXE_NAME}.bin
    COMMAND ${DEPLOY_TOOL} -bin ${EXE_NAME}.bin -control -wipe /dev/$$ISP_PORT 115200 12000
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    COMMENT "Flashing binary image !")

