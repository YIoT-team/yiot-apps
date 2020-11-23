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

cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(yiot-common VERSION 0.1.0 LANGUAGES C)

# ---------------------------------------------------------------------------
#	IoTKit
# ---------------------------------------------------------------------------

set (IOTKIT_PATH "${CMAKE_CURRENT_LIST_DIR}/../ext/iotkit")
set (CMAKE_MODULE_PATH "${IOTKIT_PATH}/sdk/cmake" ${CMAKE_MODULE_PATH})

include(TransitiveToolchainArgs)
include(helpers)

#   Configure
set (VIRGIL_IOT_CONFIG_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/config/pc")
#set(GO_DISABLE CACHE BOOL ON FORCE)
option(ENABLE_TESTING OFF)
option(ENABLE_HEAVY_TESTS OFF)
option(VIRGIL_IOT_CLOUD OFF)
option(VIRGIL_IOT_HIGH_LEVEL OFF)
option(VIRGIL_IOT_THREADSAFE ON)
option(VIRGIL_IOT_DEFAULT_CLOUD_CURL_HTTP OFF)
option(VIRGIL_IOT_DEFAULT_CLOUD_MESSAGE_BIN_AWS OFF)

#   Add
add_subdirectory(${IOTKIT_PATH}/sdk iotkit)
add_subdirectory(${IOTKIT_PATH}/demo/nix/common iotkit-nix-common)

# ---------------------------------------------------------------------------
#	IoTKit extensions
# ---------------------------------------------------------------------------
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/protocols/snap")

