#!/bin/sh
echo "Make the build directory"
mkdir build
echo "enter pico-sdk"
cd pico-sdk
echo "export PATH"
export PICO_SDK_PATH=$(pwd)
cd ..
cd build
CXXFLAGS="-DBOARD_CONFIRMATION_NEEDED" CFLAGS="-DBOARD_CONFIRMATION_NEEDED" cmake .. -DEXTRA_MACROS=BOARD_CONFIRMATION_NEEDED
make -j16
