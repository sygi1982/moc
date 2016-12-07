# Info
Smart CAN C++ library.

# Setup for embedded target

export PATH=$PATH:/opt/arm-cross-compilers/gcc-arm-none-eabi-5_4-2016q2/bin/

# Build

mkdir build;

cd build;

cmake -DCMAKE_BUILD_TYPE=Debug -DTARGET_OS=linux ..  or

cmake -DCMAKE_BUILD_TYPE=Debug -DTARGET_OS=linux -DCMAKE_TOOLCHAIN_FILE=../tools/arm_cross_linux.cmake ..  or

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../tools/arm_cross_baremetal.cmake -DTARGET_OS=baremetal -DTARGET_PLAT=lpc17xx ..

make;

# Tests

## Build UnitTests (gtest)

cmake -DCMAKE_BUILD_TYPE=Debug -DTARGET_OS=linux -DBUILD_UT=ON ..; make;

## Functional tests

python,bash - TBD

# Deploy for embedded target

make deploy ISP_PORT=ttyS1
