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
set(KS_TARGET_VERSION "0.0.2")

# ---------------------------------------------------------------------------
# Define project
# ---------------------------------------------------------------------------
project(yiot VERSION ${KS_TARGET_VERSION} LANGUAGES C CXX)

# ---------------------------------------------------------------------------
# Include Cmake helpers
# ---------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(vsplatforms)
include(varpadding)
#include(clangformat)

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
#   Options
# ---------------------------------------------------------------------------
option(BUILD_DEVICE_ENABLE "Enable building of Device app" ON)
option(BUILD_APP_ENABLE "Enable building of Application" ON)


# ---------------------------------------------------------------------------
#   Platform limitations
# ---------------------------------------------------------------------------
if (NOT KS_PLATFORM STREQUAL "linux")
    set (BUILD_DEVICE_ENABLE OFF)
endif()


# ---------------------------------------------------------------------------
#	Common libraries
# ---------------------------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/common)

# ---------------------------------------------------------------------------
#	Device Applications
# ---------------------------------------------------------------------------
if (BUILD_DEVICE_ENABLE)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/device-app)
endif()

# ---------------------------------------------------------------------------
#	Cross-platform control applications
# ---------------------------------------------------------------------------
if (BUILD_APP_ENABLE)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/controll-app)
endif()
