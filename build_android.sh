#!/bin/bash

set -e

rm -rf build
mkdir -p build

pushd build
  cmake -DVS_PLATFORM="android" -DANDROID_PLATFORM=29 -DANDROID_QT=ON ..
popd  
