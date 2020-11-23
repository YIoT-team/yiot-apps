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

function realpath() {
    [[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}

PNG_FILE=$(realpath ${1})
PNG_FILE_DIR="$( cd "$( dirname "$PNG_FILE}" )" >/dev/null 2>&1 && pwd )"
PNG_FILE_NAME=$(basename ${PNG_FILE})
PNG_FILE_NAME_BASE=${PNG_FILE_NAME%.png}


if [ ! -f ${PNG_FILE} ]; then
  echo "PNG file doesn't exist."
  exit 1
fi

echo "PNG_FILE           = ${PNG_FILE}"
echo "PNG_FILE_DIR       = ${PNG_FILE_DIR}"
echo "PNG_FILE_NAME      = ${PNG_FILE_NAME}"
echo "PNG_FILE_NAME_BASE = ${PNG_FILE_NAME_BASE}"

pushd "${PNG_FILE_DIR}"
  BASE_DIR="${PNG_FILE_NAME_BASE}.iconset"
  mkdir "${BASE_DIR}"
  sips -z 16 16     "${PNG_FILE_NAME}" --out ${BASE_DIR}/icon_16x16.png
  sips -z 32 32     "${PNG_FILE_NAME}" --out ${BASE_DIR}/icon_16x16@2x.png
  sips -z 32 32     "${PNG_FILE_NAME}" --out ${BASE_DIR}/icon_32x32.png
  sips -z 64 64     "${PNG_FILE_NAME}" --out ${BASE_DIR}/icon_32x32@2x.png
  sips -z 128 128   "${PNG_FILE_NAME}" --out ${BASE_DIR}/icon_128x128.png
  sips -z 256 256   "${PNG_FILE_NAME}" --out ${BASE_DIR}/icon_128x128@2x.png
  sips -z 256 256   "${PNG_FILE_NAME}" --out ${BASE_DIR}/icon_256x256.png
  sips -z 512 512   "${PNG_FILE_NAME}" --out ${BASE_DIR}/icon_256x256@2x.png
  sips -z 512 512   "${PNG_FILE_NAME}" --out ${BASE_DIR}/icon_512x512.png
  cp "${PNG_FILE_NAME}" ${BASE_DIR}/icon_512x512@2x.png
  iconutil -c icns ${BASE_DIR}
  rm -R ${BASE_DIR}
popd