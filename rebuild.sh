#!/bin/bash

BUILD_DIR=./build
BUILD_TYPE=release

mkdir -p ${BUILD_DIR}
rm -rf ${BUILD_DIR}/*
cd ${BUILD_DIR}
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DBUILD_TESTING=1 ..

make -j$((`nproc` / 2))
#cpack
