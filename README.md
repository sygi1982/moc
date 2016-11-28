# Info
Smart CAN C++ library.

# Setup

export PATH=$PATH:/opt/gcc-arm-none-eabi-5_4-2016q2/bin/

# Build

mkdir build;
cd build;
cmake -DCMAKE_BUILD_TYPE=Debug -DTARGET_OS=linux ..  or
cmake -DCMAKE_BUILD_TYPE=Debug -DTARGET_OS=linux -DCMAKE_TOOLCHAIN_FILE=../tools/arm_cross.cmake ..  or
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../tools/arm_cross.cmake -DTARGET_OS=baremetal -DTARGET_PLAT=lpc17xx ..
make;

# Deploy for embedded target

make deploy ISP_PORT=ttyS1
