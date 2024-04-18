#!/bin/sh
echo "Make the build directory"
mkdir build
echo "enter pico-sdk"
cd pico-sdk
echo "export PATH"
export PICO_SDK_PATH=$(pwd)
cd ..
cd build
cmake ..
make