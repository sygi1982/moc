# moc
Modbus over CAN library

# setup

export PATH=$PATH:/opt/gcc-arm-none-eabi-5_4-2016q2/bin/

# build

mkdir build;
cd build;
cmake -DCMAKE_BUILD_TYPE=Debug -DTARGET_PLAT=x86 ..  or
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../plat/arm/cross.cmake -DTARGET_PLAT=arm ..
make;

