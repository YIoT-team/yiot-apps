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

project(iotkit-qt-lib VERSION 0.1.0 LANGUAGES C CXX)

# ---------------------------------------------------------------------------
#   C++
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ---------------------------------------------------------------------------
#   Qt
# ---------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Core5Compat Core Network Concurrent Bluetooth Qml)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---------------------------------------------------------------------------
#   Variables
# ---------------------------------------------------------------------------
set(IOTKIT_QT_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../ext/yiot-core/iotkit/integration/qt")

set(FACADE_PATH "${IOTKIT_QT_PATH}/facade")
set(DEFAULT_IMPL_PATH "${IOTKIT_QT_PATH}/default-impl")
set(DEFAULT_IMPL_NETIF_PATH "${DEFAULT_IMPL_PATH}/netif")

set(INC_HELPERS "${FACADE_PATH}/include/virgil/iot/qt/helpers")
set(INC_SNAP "${FACADE_PATH}/include/virgil/iot/qt/protocols/snap")
set(INC_BLE_NETIF "${DEFAULT_IMPL_NETIF_PATH}/ble/include/virgil/iot/qt/netif")

set(SRC_HELPERS "${FACADE_PATH}/src/helpers")
set(SRC_SNAP "${FACADE_PATH}/src")

# ---------------------------------------------------------------------------
#   IoTKit Qt wrapper lib
# ---------------------------------------------------------------------------
add_library(iotkit-qt-lib)

# ---------------------------------------------------------------------------
#	Header/Source files
# ---------------------------------------------------------------------------
target_sources(iotkit-qt-lib
        PRIVATE

        #   Headers
        ${INC_BLE_NETIF}/VSQNetifBLE.h
        ${INC_BLE_NETIF}/VSQNetifBLEEnumerator.h
        ${INC_HELPERS}/VSQAppConfig.h
        ${INC_HELPERS}/VSQDeviceRoles.h
        ${INC_HELPERS}/VSQDeviceSerial.h
        ${INC_HELPERS}/VSQDeviceType.h
        ${INC_HELPERS}/VSQFileVersion.h
        ${INC_HELPERS}/VSQImplementations.h
        ${INC_HELPERS}/VSQMac.h
        ${INC_HELPERS}/VSQManufactureId.h
        ${INC_HELPERS}/VSQSingleton.h
        ${INC_HELPERS}/VSQHelpers.h
        ${INC_HELPERS}/../VSQIoTKit.h
        ${INC_SNAP}/VSQNetifBase.h
        ${INC_SNAP}/VSQSnapServiceBase.h
        ${INC_SNAP}/VSQSnapINFOClient.h
        ${INC_SNAP}/VSQSnapINFOClientQml.h
        ${INC_SNAP}/VSQSnapSnifferQml.h
        ${INC_SNAP}/VSQSnapCFGClient.h

        #   Sources
        ${IOTKIT_QT_PATH}/default-impl/netif/ble/src/VSQNetifBLE.cpp
        ${IOTKIT_QT_PATH}/default-impl/netif/ble/src/VSQNetifBLEEnumerator.cpp
        ${IOTKIT_QT_PATH}/default-impl/hal.cpp
        ${SRC_HELPERS}/VSQDeviceRoles.cpp
        ${SRC_HELPERS}/VSQDeviceSerial.cpp
        ${SRC_HELPERS}/VSQDeviceType.cpp
        ${SRC_HELPERS}/VSQFileVersion.cpp
        ${SRC_HELPERS}/VSQMac.cpp
        ${SRC_HELPERS}/VSQManufactureId.cpp
        ${SRC_SNAP}/VSQNetifBase.cpp
        ${SRC_SNAP}/VSQSnapINFOClient.cpp
        ${SRC_SNAP}/VSQSnapCFGClient.cpp
        ${SRC_SNAP}/VSQSnapINFOClientQml.cpp
        ${SRC_SNAP}/VSQSnapSnifferQml.cpp
        )


# ---------------------------------------------------------------------------
#	Definitions
# ---------------------------------------------------------------------------
target_compile_definitions(iotkit-qt-lib
        PUBLIC
        INFO_SERVER=1
        CFG_CLIENT=1
        )


# ---------------------------------------------------------------------------
#   Include directories
# ---------------------------------------------------------------------------
target_include_directories(iotkit-qt-lib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<BUILD_INTERFACE:${FACADE_PATH}/include>
        $<BUILD_INTERFACE:${DEFAULT_IMPL_NETIF_PATH}/ble/include>
        )


# ---------------------------------------------------------------------------
#	Compile options
# ---------------------------------------------------------------------------
target_compile_options(iotkit-qt-lib
        PUBLIC
        -Wno-multichar
        $<$<BOOL:${WIN32}>:-mno-ms-bitfields -mno-ms-bitfields>
        )


# ---------------------------------------------------------------------------
#	Link libraries
# ---------------------------------------------------------------------------
target_link_libraries(iotkit-qt-lib
        PUBLIC

        #   Qt6
        Qt6::Core
        Qt6::Core5Compat
        Qt6::Network
        Qt6::Concurrent
        Qt6::Bluetooth
        Qt6::Qml

        #   IoTKit
        vs-module-logger
        vs-module-provision
        vs-module-snap-control

        #   IoTKit extensions
        yiot-snap-control

        #   Platform specific
        $<$<BOOL:${WIN32}>:ws2_32>

        # Compiler options
        # enable_pedantic_mode
        )

