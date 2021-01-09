#!/bin/bash
set -e

readonly SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
readonly PROJECT_DIR="${SCRIPT_DIR}/../../"

rm -rf ${PROJECT_DIR}/build

build_arch() {
    PARAM_ABI="${1}"
    rm -rf ${PROJECT_DIR}/build/${PARAM_ABI}
    mkdir -p ${PROJECT_DIR}/build/${PARAM_ABI}
    pushd ${PROJECT_DIR}/build/${PARAM_ABI}
	cmake -DCMAKE_BUILD_TYPE="MinSizeRel" -DKS_PLATFORM="android" -DANDROID_PLATFORM=29 -DANDROID_QT=ON -DANDROID_ABI="${PARAM_ABI}" ../..
	make yiot    
    popd
}

build_apk() {
    rm -rf ${PROJECT_DIR}/build/all
    mkdir -p ${PROJECT_DIR}/build/all
    pushd ${PROJECT_DIR}/build/all
	cmake -DCMAKE_BUILD_TYPE="MinSizeRel" -DKS_PLATFORM="android" -DANDROID_PLATFORM=29 -DANDROID_QT=ON \
	    -DVS_KEYCHAIN="${VS_KEYCHAIN}" \
            -DVS_KEYCHAIN_PASSWORD="${VS_KEYCHAIN_PASSWORD}" \
            -DVS_KEY_ALIAS="${VS_KEY_ALIAS}" \
            -DVS_KEY_PASSWORD="${VS_KEY_PASSWORD}" \
             ../..

	cp -rf ../arm64-v8a/android-build/libs  ./android-build/
    cp -rf ../armeabi-v7a/android-build/libs  ./android-build/
    cp -rf ../x86/android-build/libs    ./android-build/
    cp -rf ../x86_64/android-build/libs ./android-build/

	cp -rf ../armv7/common/iotkit/modules/crypto/converters/*.so ./common/iotkit/modules/crypto/converters/
    cp -rf ../armv8/common/iotkit/modules/crypto/converters/*.so ./common/iotkit/modules/crypto/converters/
    cp -rf ../x86/common/iotkit/modules/crypto/converters/*.so ./common/iotkit/modules/crypto/converters/
    cp -rf ../x86_64/common/iotkit/modules/crypto/converters/*.so ./common/iotkit/modules/crypto/converters/

	make apk_release
	make aab_release	
    popd

}

build_arch "x86"
build_arch "x86_64"
build_arch "armeabi-v7a"
build_arch "arm64-v8a"
build_apk