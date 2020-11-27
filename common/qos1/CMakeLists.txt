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

project(yiot-qos1-helper VERSION 0.1.0 LANGUAGES C CXX)

# ---------------------------------------------------------------------------
#   C++
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ---------------------------------------------------------------------------
#	QoS1 helper library
# ---------------------------------------------------------------------------
add_library(yiot-qos1-helper)


# ---------------------------------------------------------------------------
#	Header/Source files
# ---------------------------------------------------------------------------
target_sources(yiot-qos1-helper
        PRIVATE
        # Headers
        ${CMAKE_CURRENT_LIST_DIR}/include/qos1/resend-container.h

        # Sources
        ${CMAKE_CURRENT_LIST_DIR}/src/resend-container.cpp
        )


# ---------------------------------------------------------------------------
#	Definitions
# ---------------------------------------------------------------------------
target_compile_definitions(yiot-qos1-helper
        PRIVATE
        )


# ---------------------------------------------------------------------------
#   Include directories
# ---------------------------------------------------------------------------
target_include_directories(yiot-qos1-helper
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        )


# ---------------------------------------------------------------------------
#	Compile options
# ---------------------------------------------------------------------------
target_compile_options(yiot-qos1-helper
        PRIVATE
        -Wno-multichar
        )


# ---------------------------------------------------------------------------
#	Link libraries
# ---------------------------------------------------------------------------
target_link_libraries(yiot-qos1-helper
        PRIVATE

        #   IoTKit
        nix-common
        vs-module-snap-thing

        #   Helpers
        enable_sanitizers
        #       enable_pedantic_mode
        )


# ---------------------------------------------------------------------------
#   Format code
# ---------------------------------------------------------------------------
if (COMMAND add_clangformat)
    add_clangformat(yiot-qos1-helper)
endif ()


#------------------------------------------------------------------------------
#   Install
# ---------------------------------------------------------------------------
install(TARGETS yiot-qos1-helper
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
