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

project(yiot-device-app-emulator VERSION 0.1.0 LANGUAGES C CXX)

# ---------------------------------------------------------------------------
#   C++
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
#	Add dependencies
# ---------------------------------------------------------------------------

#   Ext projects path
set(EXT_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../ext")

# ---------------------------------------------------------------------------
#	YIoT device emulator
# ---------------------------------------------------------------------------
add_executable(yiot-device-app-emulator)

# ---------------------------------------------------------------------------
#	Header/Source files
# ---------------------------------------------------------------------------
set(COMMON_NIX_DIR ${CMAKE_CURRENT_LIST_DIR}/../common/nix)
target_sources(yiot-device-app-emulator
        PRIVATE
        # Headers
        ${CMAKE_CURRENT_LIST_DIR}/include/init.h
        ${CMAKE_CURRENT_LIST_DIR}/include/settings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/commands/device-info.h
        ${CMAKE_CURRENT_LIST_DIR}/include/commands/pc.h
        ${CMAKE_CURRENT_LIST_DIR}/include/commands/reset.h
        ${COMMON_NIX_DIR}/include/common/helpers/timer.h
        ${COMMON_NIX_DIR}/include/common/helpers/command.h
        ${COMMON_NIX_DIR}/include/common/iotkit-impl/netif/curl-websock.h
        ${COMMON_NIX_DIR}/include/common/iotkit-impl/netif/netif-websock.h

        # Sources
        ${CMAKE_CURRENT_LIST_DIR}/src/main.c
        ${CMAKE_CURRENT_LIST_DIR}/src/init.c
        ${CMAKE_CURRENT_LIST_DIR}/src/settings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/commands/device-info.c
        ${CMAKE_CURRENT_LIST_DIR}/src/commands/pc.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/commands/reset.cpp
        ${COMMON_NIX_DIR}/src/helpers/timer.cpp
        ${COMMON_NIX_DIR}/src/iotkit-impl/helpers.c
        ${COMMON_NIX_DIR}/src/iotkit-impl/netif/curl-websock.c
        ${COMMON_NIX_DIR}/src/iotkit-impl/netif/netif-websock.c
        )


# ---------------------------------------------------------------------------
#	Definitions
# ---------------------------------------------------------------------------
target_compile_definitions(yiot-device-app-emulator
        PRIVATE
        )


# ---------------------------------------------------------------------------
#   Include directories
# ---------------------------------------------------------------------------
target_include_directories(yiot-device-app-emulator
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${COMMON_NIX_DIR}/include
        )


# ---------------------------------------------------------------------------
#	Compile options
# ---------------------------------------------------------------------------
target_compile_options(yiot-device-app-emulator
        PRIVATE
        -Wno-multichar
        )

# ---------------------------------------------------------------------------
#	Link libraries
# ---------------------------------------------------------------------------
target_link_libraries(yiot-device-app-emulator
        PRIVATE

        #   IoTKit
        nix-common
        vs-module-snap-thing
        vs-module-snap-initializer
        vs-json
        vs-base64

        #   IoTKit extensions
        yiot-snap-thing-pc
        yiot-qos1-helper

        #   JSON
        nlohmann_json::nlohmann_json

        #   CURL
        curl

        #   Helpers
        enable_sanitizers
        #       enable_pedantic_mode
        )


# ---------------------------------------------------------------------------
#   Format code
# ---------------------------------------------------------------------------
if (COMMAND add_clangformat)
    add_clangformat(yiot-device-app-emulator)
endif ()


# ---------------------------------------------------------------------------
#   Install
# ---------------------------------------------------------------------------
install(TARGETS yiot-device-app-emulator
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
