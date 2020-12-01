#!/bin/bash

#  ────────────────────────────────────────────────────────────
#                     ╔╗  ╔╗ ╔══╗      ╔════╗
#                     ║╚╗╔╝║ ╚╣╠╝      ║╔╗╔╗║
#                     ╚╗╚╝╔╝  ║║  ╔══╗ ╚╝║║╚╝
#                      ╚╗╔╝   ║║  ║╔╗║   ║║
#                       ║║   ╔╣╠╗ ║╚╝║   ║║
#                       ╚╝   ╚══╝ ╚══╝   ╚╝
#    ╔╗╔═╗                    ╔╗                     ╔╗
#    ║║║╔╝                   ╔╝╚╗                    ║║
#    ║╚╝╝  ╔══╗ ╔══╗ ╔══╗  ╔╗╚╗╔╝  ╔══╗ ╔╗ ╔╗╔╗ ╔══╗ ║║  ╔══╗
#    ║╔╗║  ║║═╣ ║║═╣ ║╔╗║  ╠╣ ║║   ║ ═╣ ╠╣ ║╚╝║ ║╔╗║ ║║  ║║═╣
#    ║║║╚╗ ║║═╣ ║║═╣ ║╚╝║  ║║ ║╚╗  ╠═ ║ ║║ ║║║║ ║╚╝║ ║╚╗ ║║═╣
#    ╚╝╚═╝ ╚══╝ ╚══╝ ║╔═╝  ╚╝ ╚═╝  ╚══╝ ╚╝ ╚╩╩╝ ║╔═╝ ╚═╝ ╚══╝
#                    ║║                         ║║
#                    ╚╝                         ╚╝
#
#    Lead Maintainer: Roman Kutashenko <kutashenko@gmail.com>
#  ────────────────────────────────────────────────────────────

readonly SCRIPT_FOLDER="$(cd "$(dirname "$0")" && pwd)"

export QTDIR=/home/roman/soft/Qt5/5.15.1/gcc_64
export ANDROID_NDK=/home/roman/Android/Sdk/ndk/21.1.6352462
export ANDROID_SDK=/home/roman/Android/Sdk
export JAVA_HOME=/home/roman/soft/jdk-8u271-linux-x64/jdk1.8.0_271

VS_KEYCHAIN="${1}"
VS_KEYCHAIN_PASSWORD="${2}"
VS_KEY_PASSWORD="${3}"

pushd "${SCRIPT_FOLDER}/../../"
     BUILD_DIR="build"
    if [ -d ${BUILD_DIR} ]; then
        rm -rf ${BUILD_DIR}
    fi
    mkdir ${BUILD_DIR}

    mkdir "${BUILD_DIR}/armv7"
    pushd "${BUILD_DIR}/armv7"
        cmake -DKS_PLATFORM="android" \
            -DCMAKE_PREFIX_PATH="/home/roman/soft/Qt5/5.15.1/android" \
            -DCMAKE_TOOLCHAIN_FILE=/home/roman/Android/Sdk/ndk/21.1.6352462/build/cmake/android.toolchain.cmake \
            -DANDROID_PLATFORM=29 \
            -DANDROID_QT=ON \
            -DANDROID_ABI="armeabi-v7a" \
            -DANDROID_BUILD_ABI_arm64-v8a=OFF \
            -DANDROID_BUILD_ABI_armeabi-v7a=ON \
            -DANDROID_BUILD_ABI_x86=OFF \
            -DANDROID_BUILD_ABI_x86_64=OFF \
            ../..

        make -j10 yiot
    popd

    mkdir "${BUILD_DIR}/armv8"
    pushd "${BUILD_DIR}/armv8"
        cmake -DKS_PLATFORM="android" \
            -DCMAKE_PREFIX_PATH="/home/roman/soft/Qt5/5.15.1/android" \
            -DANDROID_PLATFORM=29 \
            -DANDROID_QT=ON \
            -DANDROID_ABI="arm64-v8a" \
            -DANDROID_BUILD_ABI_arm64-v8a=ON \
            -DANDROID_BUILD_ABI_armeabi-v7a=OFF \
            -DANDROID_BUILD_ABI_x86=OFF \
            -DANDROID_BUILD_ABI_x86_64=OFF \
            -DCMAKE_TOOLCHAIN_FILE=/home/roman/Android/Sdk/ndk/21.1.6352462/build/cmake/android.toolchain.cmake \
            ../..
        make -j10 yiot
    popd

    mkdir "${BUILD_DIR}/x86"
    pushd "${BUILD_DIR}/x86"
        cmake -DKS_PLATFORM="android" \
            -DCMAKE_PREFIX_PATH="/home/roman/soft/Qt5/5.15.1/android" \
            -DCMAKE_TOOLCHAIN_FILE=/home/roman/Android/Sdk/ndk/21.1.6352462/build/cmake/android.toolchain.cmake \
            -DANDROID_PLATFORM=29 \
            -DANDROID_QT=ON \
            -DANDROID_ABI="x86" \
            -DANDROID_BUILD_ABI_arm64-v8a=OFF \
            -DANDROID_BUILD_ABI_armeabi-v7a=OFF \
            -DANDROID_BUILD_ABI_x86=ON \
            -DANDROID_BUILD_ABI_x86_64=OFF \
            ../..
        make -j10 yiot
    popd

    mkdir "${BUILD_DIR}/x86_64"
    pushd "${BUILD_DIR}/x86_64"
        cmake -DKS_PLATFORM="android" \
            -DCMAKE_PREFIX_PATH="/home/roman/soft/Qt5/5.15.1/android" \
            -DCMAKE_TOOLCHAIN_FILE=/home/roman/Android/Sdk/ndk/21.1.6352462/build/cmake/android.toolchain.cmake \
            -DANDROID_PLATFORM=29 \
            -DANDROID_QT=ON \
            -DANDROID_ABI="x86_64" \
            -DANDROID_BUILD_ABI_arm64-v8a=OFF \
            -DANDROID_BUILD_ABI_armeabi-v7a=OFF \
            -DANDROID_BUILD_ABI_x86=OFF \
            -DANDROID_BUILD_ABI_x86_64=ON \
            ../..
        make -j10 yiot
    popd

    if [ -d "${BUILD_DIR}/all" ]; then
        rm -rf "${BUILD_DIR}/all"
    fi

    mkdir "${BUILD_DIR}/all"
    pushd "${BUILD_DIR}/all"
        cmake -DKS_PLATFORM="android" \
            -DCMAKE_PREFIX_PATH="/home/roman/soft/Qt5/5.15.1/android" \
            -DCMAKE_TOOLCHAIN_FILE=/home/roman/Android/Sdk/ndk/21.1.6352462/build/cmake/android.toolchain.cmake \
            -DANDROID_PLATFORM=29 \
            -DANDROID_QT=ON \
            -DANDROID_ABI="arm64-v8a" \
            -DANDROID_BUILD_ABI_arm64-v8a=ON \
            -DANDROID_BUILD_ABI_armeabi-v7a=ON \
            -DANDROID_BUILD_ABI_x86=ON \
            -DANDROID_BUILD_ABI_x86_64=ON \
            -DVS_KEYCHAIN="${VS_KEYCHAIN}" \
            -DVS_KEYCHAIN_PASSWORD="${VS_KEYCHAIN_PASSWORD}" \
            -DVS_KEY_PASSWORD="${VS_KEY_PASSWORD}" \
            ../..

        cp -rf ../armv7/android-build/libs  ./android-build/
        cp -rf ../armv8/android-build/libs  ./android-build/
        cp -rf ../x86/android-build/libs    ./android-build/
        cp -rf ../x86_64/android-build/libs ./android-build/

        make -j10 aab_release
    popd


    #if []
    #mkdir artifacts
popd



