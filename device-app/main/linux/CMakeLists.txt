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

project(yiot-device-app-linux VERSION 0.1.0 LANGUAGES C CXX)

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

#   JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory("${EXT_PATH}/json" json)

#   Path to Bluez-DBus
set(BLUEZ_DBUS_PATH "${EXT_PATH}/bluez-dbus")

#   Build and install sdbus-cpp
virgil_depends(
        PACKAGE_NAME "sdbus"
        CONFIG_DIR "${EXT_PATH}/cmake"
)

#   Add BlueZ-DBus
unset(BUILD_SHARED_LIBS CACHE)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${VIRGIL_DEPENDS_PREFIX}/lib/cmake/sdbus-c++" "${VIRGIL_DEPENDS_PREFIX}/lib64/cmake/sdbus-c++")
add_subdirectory(${BLUEZ_DBUS_PATH} bluez-dbus)
set(BUILD_SHARED_LIBS CACHE BOOL OFF FORCE)

# ---------------------------------------------------------------------------
#	Linux BLE provisioning application
# ---------------------------------------------------------------------------
add_executable(yiot-device-app-linux)


# ---------------------------------------------------------------------------
#	Header/Source files
# ---------------------------------------------------------------------------
target_sources(yiot-device-app-linux
        PRIVATE
        # Headers
        ${CMAKE_CURRENT_LIST_DIR}/include/iotkit-impl/init.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iotkit-impl/netif/netif-ble-linux.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iotkit-impl/netif/netif-udp.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iotkit-impl/netif/curl-websock.h
        ${CMAKE_CURRENT_LIST_DIR}/include/iotkit-impl/netif/netif-websock.h
        ${CMAKE_CURRENT_LIST_DIR}/include/commands/device-info.h
        ${CMAKE_CURRENT_LIST_DIR}/include/commands/wifi-cred.h
        ${CMAKE_CURRENT_LIST_DIR}/include/commands/pc.h
        ${CMAKE_CURRENT_LIST_DIR}/include/commands/reset.h
        ${CMAKE_CURRENT_LIST_DIR}/include/helpers/timer.h
        ${CMAKE_CURRENT_LIST_DIR}/include/helpers/command.h
        ${CMAKE_CURRENT_LIST_DIR}/include/helpers/settings.h
        ${CMAKE_CURRENT_LIST_DIR}/include/helpers/utils.h

        # Sources
        ${CMAKE_CURRENT_LIST_DIR}/src/main.c
        ${CMAKE_CURRENT_LIST_DIR}/src/commands/device-info.c
        ${CMAKE_CURRENT_LIST_DIR}/src/commands/wifi-cred.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/commands/pc.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/commands/reset.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/helpers/timer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/helpers/settings.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/helpers/utils.c
        ${CMAKE_CURRENT_LIST_DIR}/src/iotkit-impl/helpers.c
        ${CMAKE_CURRENT_LIST_DIR}/src/iotkit-impl/init.c
        ${CMAKE_CURRENT_LIST_DIR}/src/iotkit-impl/netif/netif-ble-linux.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/iotkit-impl/netif/netif-udp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/iotkit-impl/netif/curl-websock.c
        ${CMAKE_CURRENT_LIST_DIR}/src/iotkit-impl/netif/netif-websock.c
        )


# ---------------------------------------------------------------------------
#	Definitions
# ---------------------------------------------------------------------------
target_compile_definitions(yiot-device-app-linux
        PRIVATE
        )


# ---------------------------------------------------------------------------
#   Include directories
# ---------------------------------------------------------------------------
target_include_directories(yiot-device-app-linux
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        )


# ---------------------------------------------------------------------------
#	Compile options
# ---------------------------------------------------------------------------
target_compile_options(yiot-device-app-linux
        PRIVATE
        -Wno-multichar
        )

# ---------------------------------------------------------------------------
#	Link libraries
# ---------------------------------------------------------------------------
target_link_libraries(yiot-device-app-linux
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

        #   Bluez DBus
        bluez-dbus-cpp

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
    add_clangformat(yiot-device-app-linux)
endif ()


# ---------------------------------------------------------------------------
#   Install
# ---------------------------------------------------------------------------
install(TARGETS yiot-device-app-linux
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
