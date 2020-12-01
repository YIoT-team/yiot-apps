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

project(yiot-snap VERSION 0.1.0 LANGUAGES C)

function(add_snap _targetname _definitions)

    #
    #   SNAP library
    #
    add_library(${_targetname})

    #
    #   Sources
    #
    target_sources(${_targetname}
            PRIVATE

            ${CMAKE_CURRENT_LIST_DIR}/include/common/protocols/snap/lamp/lamp-server.h
            ${CMAKE_CURRENT_LIST_DIR}/include/common/protocols/snap/lamp/lamp-client.h
            ${CMAKE_CURRENT_LIST_DIR}/include/common/protocols/snap/pc/pc-server.h
            ${CMAKE_CURRENT_LIST_DIR}/include/common/protocols/snap/pc/pc-client.h

            ${CMAKE_CURRENT_LIST_DIR}/src/lamp/lamp-server.c
            ${CMAKE_CURRENT_LIST_DIR}/src/lamp/lamp-client.c
            ${CMAKE_CURRENT_LIST_DIR}/src/pc/pc-server.c
            ${CMAKE_CURRENT_LIST_DIR}/src/pc/pc-client.c
            )

    #
    #   Common include directories
    #
    target_include_directories(${_targetname}
            PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

            PRIVATE
            $<BUILD_INTERFACE:${VIRGIL_IOT_CONFIG_DIRECTORY}>
            $<BUILD_INTERFACE:${IOTKIT_PATH}/sdk/modules/protocols/snap/include>
            )
    install(TARGETS ${_targetname}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )

    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )

    #
    #   Add definitions
    #
    if ("${_definitions}" STREQUAL "")
        message("-- No _definitions for SNAP target ${_targetname}")
    else()
        message("-- SNAP_DEFINITIONS for ${_targetname} = ${_definitions}")
        string(REPLACE " " ";" SNAP_DEFINITIONS ${_definitions})
    endif()

    target_compile_definitions(${_targetname}
            PUBLIC
            ${SNAP_DEFINITIONS}
            )

    #
    #   Link libraries
    #
    target_link_libraries(${_targetname}
            PUBLIC
            virgil-iot-status-code
            vs-module-provision

            PRIVATE
            macros
            enable_pedantic_mode
            )

    #
    #   Format code
    #
    if (COMMAND add_clangformat)
        add_clangformat(${_targetname})
    endif ()

endfunction()

#
#   SNAP For Thing Lamp
#
add_snap("yiot-snap-thing-lamp" "LAMP_SERVER=1")

#
#   SNAP For Thing PC
#
add_snap("yiot-snap-thing-pc" "PC_SERVER=1")

#
#   SNAP For Control
#
add_snap("yiot-snap-control" "LAMP_CLIENT=1 PC_CLIENT=1")
