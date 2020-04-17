#!env bash

if [ "$2" == "debug" ]; then
    cmake -DCMAKE_C_COMPILER=/usr/bin/$1 -DCMAKE_CXX_COMPILER=/usr/bin/$1++ -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_CXX_FLAGS_DEBUG="-Wall -g -O0" $3
fi

if [ "$2" == "release" ]; then
    cmake -DCMAKE_C_COMPILER=/usr/bin/$1 -DCMAKE_CXX_COMPILER=/usr/bin/$1++ -DCMAKE_BUILD_TYPE="Release" -DCMAKE_CXX_FLAGS_DEBUG="-Wall -O3" $3
fi
