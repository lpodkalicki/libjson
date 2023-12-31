#!/bin/bash

BUILD_DIR=./build
BUILD_TYPE=release
BUILD_TESTING=1
BUILD_EXAMPLES=1

mkdir -p ${BUILD_DIR}
rm -rf ${BUILD_DIR}/*
cd ${BUILD_DIR}
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DBUILD_TESTING=${BUILD_TESTING} -DBUILD_EXAMPLES=${BUILD_EXAMPLES} ..

make -j6

