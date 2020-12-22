#!/bin/bash

set -e

rm -rf build
mkdir -p build

pushd build
    cmake -DKS_PLATFORM="android" -DANDROID_PLATFORM=29 -DANDROID_QT=ON -DANDROID=ON ..
    make
popd  
