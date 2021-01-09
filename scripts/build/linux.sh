#!/bin/bash
set -e

readonly SCRIPT_DIR="$(cd $(dirname "$0") && pwd)"
readonly PROJECT_DIR="${SCRIPT_DIR}/../../"

rm -rf ${PROJECT_DIR}/build
mkdir -p ${PROJECT_DIR}/build
pushd ${PROJECT_DIR}/build
	cmake -DCMAKE_BUILD_TYPE="MinSizeRel" -DKS_PLATFORM="linux" ..
	make yiot
popd
