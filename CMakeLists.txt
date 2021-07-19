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

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# ---------------------------------------------------------------------------
# Project version
# ---------------------------------------------------------------------------
set(KS_TARGET_VERSION "0.1.0")

# ---------------------------------------------------------------------------
#   Options
# ---------------------------------------------------------------------------
option(BUILD_DEVICE_ENABLE "Enable building of Device app" ON)
option(BUILD_EMULATORS_ENABLE "Enable building of Device emulators" ON)
option(BUILD_APP_ENABLE "Enable building of Application" ON)

# ---------------------------------------------------------------------------
# Include Cmake helpers
# ---------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if (BUILD_APP_ENABLE)
    include(vsplatforms)
endif()
include(varpadding)
#include(clangformat)

# ---------------------------------------------------------------------------
# Define project
# ---------------------------------------------------------------------------

project(yiot VERSION ${KS_TARGET_VERSION} LANGUAGES C CXX)
file(WRITE "${CMAKE_BINARY_DIR}/VERSION" "${KS_TARGET_VERSION}")
# ---------------------------------------------------------------------------
#   Detecting platform if KS_PLATFORM not specified
# ---------------------------------------------------------------------------
if(NOT KS_PLATFORM)
    if(ANDROID)
        message(STATUS "Detected target platform: [Android]")
        set(KS_PLATFORM "android")
    elseif(UNIX)
        message(STATUS "Detected target platform: [Unix]")
        set(KS_PLATFORM "linux")
    elseif(MACOS)
        message(STATUS "Detected target platform: [MacOS]")
        set(KS_PLATFORM "macos")
    elseif(IOS)
        message(STATUS "Detected target platform: [IOS]")
        set(KS_PLATFORM "ios")
    elseif(WIN32)
        message(STATUS "Detected target platform: [Windows]")
        set(KS_PLATFORM "windows")
    endif()
endif()


if ((KS_PLATFORM STREQUAL "linux") OR (KS_PLATFORM STREQUAL "macos") OR (KS_PLATFORM STREQUAL "windows"))
    set (KS_PLATFORM_DESKTOP ON)
else()
    set (KS_PLATFORM_DESKTOP OFF)
endif()

message(STATUS "KS_PLATFORM: ${KS_PLATFORM}")
message(STATUS "KS_PLATFORM_DESKTOP: ${KS_PLATFORM_DESKTOP}")


# ---------------------------------------------------------------------------
#   Platform limitations
# ---------------------------------------------------------------------------
if (NOT KS_PLATFORM STREQUAL "linux")
    set (BUILD_DEVICE_ENABLE OFF)
endif()

if ((NOT KS_PLATFORM STREQUAL "linux") AND (NOT KS_PLATFORM STREQUAL "macos"))
    set (BUILD_EMULATORS_ENABLE OFF)
endif()


# ---------------------------------------------------------------------------
#	YIoT-core
# ---------------------------------------------------------------------------
set(VIRGIL_IOT_CONFIG_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/ext/yiot-core/iotkit/sdk/config/pc)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ext/yiot-core)

# ---------------------------------------------------------------------------
#	Device Applications
# ---------------------------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/device-app)

# ---------------------------------------------------------------------------
#	Cross-platform control applications
# ---------------------------------------------------------------------------
if (BUILD_APP_ENABLE)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/controll-app)
endif()
