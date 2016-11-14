# Info
Smart CAN C++ library.

# Setup

export PATH=$PATH:/opt/gcc-arm-none-eabi-5_4-2016q2/bin/

# Build

mkdir build;
cd build;
cmake -DCMAKE_BUILD_TYPE=Debug -DTARGET_ARCH=x86_64 ..  or
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../arch/arm/cross.cmake -DTARGET_ARCH=arm -DTARGET_PLAT=lpc17xx ..
make;

# Deploy for embedded target

make deploy ISP_PORT=ttyS1
