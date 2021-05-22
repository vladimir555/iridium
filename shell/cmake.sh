#!env bash

if [ "$1" == "clang" ]; then
    linker="llvm"
else
    linker="ld"
fi

if [ "$2" == "debug" ]; then
    cmake -DCMAKE_C_COMPILER=$1 -DCMAKE_CXX_COMPILER=$1++ -DCMAKE_LINKER=$linker -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_CXX_FLAGS_DEBUG="-Wall -g -O0" $3
fi

if [ "$2" == "release" ]; then
    cmake -DCMAKE_C_COMPILER=$1 -DCMAKE_CXX_COMPILER=$1++ -DCMAKE_LINKER=$linker -DCMAKE_BUILD_TYPE="Release" -DCMAKE_CXX_FLAGS_DEBUG="-Wall -O3" $3
fi
