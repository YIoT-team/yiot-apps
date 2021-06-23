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
#    Lead Maintainer: YIoT team
#  ────────────────────────────────────────────────────────────

#
#   Global variables
#

set -e
SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
SOURCE_DIR="${SCRIPT_DIR}/../.."

############################################################################################
find_tool() {
    local PARAM_CMD="${1}"
    RES_TMP="$(which ${PARAM_CMD} 2>&1)"
    if [ "${?}" != "0" ]; then
        echo "Tools [${PARAM_CMD}] NOT FOUND (Please install first)"
        return 127
    fi
    return 0
}

############################################################################################
print_message() {
    echo
    echo "=== ${1}"
}

############################################################################################
print_header() {
    echo
    echo "===================================="
    echo "=== ${1}"
    echo "===================================="
}


############################################################################################
print_usage() {
  echo
  echo "$(basename ${0})"
  echo
  echo "  -p < Package >"
  echo "  -t < Target OS  >"
  echo "  -u < macOS user  >"
  echo "  -w < macOS user password >"
  echo "  -i < macOS identity >"
  echo "  -h"
  exit 0
}
############################################################################################
#
#  Script parameters
#
############################################################################################
TARGET_OS="${1}"
if [ "${TARGET_OS}" == "" ] || [ "${TARGET_OS}" == "help" ]; then
    print_usage
    exit 0
fi

MACOS_USER=""
MACOS_PASSWORD=""
MACOS_IDENTITY=""

#
# Parse parameters
#
shift
BUILD_PKG="1"
while [ -n "$1" ]
 do
   case "$1" in
     -p) BUILD_PKG="0"
         ;;
     -t) TARGET_OS="$2"
         shift
         ;;
     -u) MACOS_USER="$2"
         shift
         ;;
     -w) MACOS_PASSWORD="$2"
         shift
         ;;
     -i) MACOS_IDENTITY="$2"
         shift
         ;;
     *) print_usage
         ;;
   esac
   shift
done

############################################################################################
build_dev_rpi() {
    print_header "Building device app for Raspberry PI"
    print_message " Remove old build directory"
    rm -rf "${SOURCE_DIR}/build" || true
    mkdir -p "${SOURCE_DIR}/build"
    export QTDIR="${QTDIR:-/opt/Qt/5.15.0/gcc_64}"
    pushd "${SOURCE_DIR}/build"
       cmake -DKS_PLATFORM="linux" -DGO_DISABLE=ON -DBUILD_APP_ENABLE=OFF ..
       make yiot-device-app-linux
    popd
    mkdir -p ${SOURCE_DIR}/build/dist/scripts
    cp -rf ${SOURCE_DIR}/device-app/main/linux/scripts/* ${SOURCE_DIR}/build/dist/scripts
    cp -rf ${SOURCE_DIR}/build/device-app/main/linux/yiot-device-app-linux ${SOURCE_DIR}/build/dist
    cp -rf ${SOURCE_DIR}/build/device-app/main/linux/bluez-dbus/libbluez-dbus-cpp.so* ${SOURCE_DIR}/build/dist
    cp -rf ${SOURCE_DIR}/build/common/iotkit/modules/crypto/converters/libconverters.so ${SOURCE_DIR}/build/dist
    if [ -f ${SOURCE_DIR}/build/depends/installed/lib/libsdbus-c++.so ]; then
        cp -rf ${SOURCE_DIR}/build/depends/installed/lib/libsdbus-c++.so* ${SOURCE_DIR}/build/dist
    else
        cp -rf ${SOURCE_DIR}/build/depends/installed/lib64/libsdbus-c++.so* ${SOURCE_DIR}/build/dist
    fi
    if [ "${BUILD_PKG}" == "1" ]; then
      find_tool dpkg-buildpackage
      if [ "${FIND_RES}" == "1" ]; then
        print_message "Build DEB package skipping"
      else
        ${SCRIPT_DIR}/pkg/prep-pkg.sh -q -b ${SOURCE_DIR}/build/dist -t dev -p deb -n yiot-rpi -v $(cat ${SOURCE_DIR}/build/VERSION).${BUILD_NUMBER:-0}
      fi
    fi
}


############################################################################################
build_app_linux() {
    print_header "Building linux application"
    
    find_tool mock || FIND_RES=1
    find_tool pbuilder || FIND_RES=1
    if [ "${FIND_RES}" == "1" ]; then
     print_message "Please install required tools"
     exit 127
    else
     print_message "OK".
    fi

    print_message " Remove old build directory"
    rm -rf "${SOURCE_DIR}/build" || true
    mkdir -p "${SOURCE_DIR}/build"
    pushd "${SOURCE_DIR}/build"
    cmake -DCMAKE_BUILD_TYPE="MinSizeRel" -DKS_PLATFORM="linux" ..
    make yiot
    make deploy
    popd
    cp -f ${SOURCE_DIR}/build/common/iotkit/modules/crypto/converters/libconverters.so ${SOURCE_DIR}/build/yiot.dist/lib
    if [ "${BUILD_PKG}" == "1" ]; then

      find_tool pbuilder
      if [ "${FIND_RES}" == "1" ]; then
        print_message "Build DEB package skipping"
      else
        sudo ${SCRIPT_DIR}/pkg/prep-pkg.sh -b ${SOURCE_DIR}/build/yiot.dist -t app -p deb -n yiot-app -v $(cat ${SOURCE_DIR}/build/VERSION).${BUILD_NUMBER:-0}
      fi

      find_tool mock
      if [ "${FIND_RES}" == "1" ]; then
        print_message "Build RPM package skipping"
      else
        sudo ${SCRIPT_DIR}/pkg/prep-pkg.sh -b ${SOURCE_DIR}/build/yiot.dist -t app -p rpm -n yiot-app -v $(cat ${SOURCE_DIR}/build/VERSION).${BUILD_NUMBER:-0}
      fi
    fi
}

############################################################################################
build_emulator_linux() {
    print_header "Building linux emulator"
    
    find_tool mock || FIND_RES=1
    if [ "${FIND_RES}" == "1" ]; then
     print_message "Please install required tools"
     exit 127
    else
     print_message "OK".
    fi

    print_message " Remove old build directory"
    rm -rf "${SOURCE_DIR}/build" || true
    mkdir -p "${SOURCE_DIR}/build"
    pushd "${SOURCE_DIR}/build"
       cmake -DCMAKE_BUILD_TYPE="MinSizeRel" -DKS_PLATFORM="linux" ..
       make yiot-device-app-emulator
    popd
    
    mkdir -p ${SOURCE_DIR}/build/yiot.dist
    cp -f ${SOURCE_DIR}/build/device-app/main/emulator/yiot-device-app-emulator ${SOURCE_DIR}/build/yiot.dist/
    cp -f ${SOURCE_DIR}/build/common/iotkit/modules/crypto/converters/libconverters.so ${SOURCE_DIR}/build/yiot.dist/
    if [ "${BUILD_PKG}" == "1" ]; then
      find_tool mock
      if [ "${FIND_RES}" == "1" ]; then
        print_message "Build RPM package skipping"
      else
        sudo ${SCRIPT_DIR}/pkg/prep-pkg.sh -b ${SOURCE_DIR}/build/yiot.dist -t app -p rpm -n yiot-emulator -v $(cat ${SOURCE_DIR}/build/VERSION).${BUILD_NUMBER:-0}
      fi
    fi
}


############################################################################################
build_app_windows() {
    print_header "Building windows application"
    
    find_tool x86_64-w64-mingw32-gcc || FIND_RES=1
    if [ "${FIND_RES}" == "1" ]; then
     print_message "Please install required tools"
     exit 127
    else
     print_message "OK".
    fi

    print_message " Remove old build directory"
    rm -rf "${SOURCE_DIR}/build" || true
    mkdir -p "${SOURCE_DIR}/build"
    pushd "${SOURCE_DIR}/build"
    cmake -DCMAKE_BUILD_TYPE="MinSizeRel" -DKS_PLATFORM="windows" -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw64.cmake ..
    make -j10 yiot
    make deploy
    popd

}

############################################################################################
build_app_macos() {
    print_header "Building macos"
    print_message " Remove old build directory"
    rm -rf "${SOURCE_DIR}/build" || true
    mkdir -p "${SOURCE_DIR}/build"
    pushd "${SOURCE_DIR}/build"
    which cmake || true
    ls -lah /usr/local/bin/cmake || true
    local CMAKE_BIN=/usr/local/bin/cmake
    ${CMAKE_BIN} --version || true
    ${CMAKE_BIN} -DCMAKE_BUILD_TYPE="MinSizeRel" -DKS_PLATFORM="macos" -DNOTARIZATION_LOGIN="${MACOS_USER}" -DNOTARIZATION_PASSWORD="${MACOS_PASSWORD}" -DVS_MACOS_IDENT="${MACOS_IDENTITY}" ..
    make -j10 yiot
    make dmg_release
#    make dmg_notarization
    popd

}



############################################################################################
case "${TARGET_OS}" in
  dev-rpi)           build_dev_rpi
                     ;;
  app-linux)         build_app_linux
                     ;;
  app-windows)       build_app_windows
                     ;;
  app-macos)         build_app_macos
                     ;;
  emulator-linux)    build_emulator_linux
                     ;;                
        *)           echo "Error build OS name"
                    exit 127
                    ;;
esac
