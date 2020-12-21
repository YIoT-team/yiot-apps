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


# ---------------------------------------------------------------------------
#   C++
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include(GNUInstallDirs)
include(FetchContent)

# ---------------------------------------------------------------------------
#   Poject Name
# ---------------------------------------------------------------------------
set (KS_TARGET_NAME yiot)

set(VS_ORGANIZATION_NAME "Kutashenko")
set(VS_ORGANIZATION_DISPLAYNAME "Kutashenko")

set(VS_APPLICATION_NAME "YIoT")
set(VS_APPLICATION_DISPLAY_NAME "YIoT")

# ---------------------------------------------------------------------------
#   Detecting build type
# ---------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER )
message("-- Build type: [${CMAKE_BUILD_TYPE}]")

# ---------------------------------------------------------------------------
# Prepare platform depends
# ---------------------------------------------------------------------------
set(KS_QT_EXTRA "")

# Get build number
if (DEFINED ENV{BUILD_NUMBER})
    set(KS_BUILD_NUMBER "$ENV{BUILD_NUMBER}")
else()
    set(KS_BUILD_NUMBER "1000")
endif()

# IOS, IOS_SIM
if(KS_PLATFORM STREQUAL "ios")
    # Android
elseif(KS_PLATFORM STREQUAL "android")

    # Add deployers targets
    include(deployers)

    # Set Android version name
    set(ANDROID_VERSION_NAME "${KS_TARGET_VERSION}")
    # Fill Android version code
    string(REPLACE "." ";" KS_VERSION_LIST ${KS_TARGET_VERSION})

    list(GET KS_VERSION_LIST 0 TMP_VERS)
    PAD_STRING(ANDROID_VERSION_CODE 3 "0" "1${TMP_VERS}")

    list(GET KS_VERSION_LIST 1 TMP_VERS)
    PAD_STRING(TMP_VERS 2 "0" "${TMP_VERS}")
    string(APPEND ANDROID_VERSION_CODE "${TMP_VERS}")

    list(GET KS_VERSION_LIST 2 TMP_VERS)
    PAD_STRING(TMP_VERS 2 "0" "${TMP_VERS}")
    string(APPEND ANDROID_VERSION_CODE "${TMP_VERS}")

    PAD_STRING(TMP_VERS 3 "0" "${KS_BUILD_NUMBER}")
    string(APPEND ANDROID_VERSION_CODE "${TMP_VERS}")
    message(STATUS "Android version name: [${ANDROID_VERSION_NAME}]")
    message(STATUS "Android version code: [${ANDROID_VERSION_CODE}]")

    # Android QT Extra
    string(APPEND KS_QT_EXTRA "AndroidExtras")
    # Android gradle path
    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/platforms/android")
endif()

# ---------------------------------------------------------------------------
#   Qt
# ---------------------------------------------------------------------------
find_package(Qt5 COMPONENTS Core Widgets Test Network Concurrent Qml ${KS_QT_EXTRA} REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---------------------------------------------------------------------------
#   Prepare for build target
# ---------------------------------------------------------------------------
# Android
if(KS_PLATFORM STREQUAL "android")
    message("-- Prepare sources for Android ...")
    add_library(${KS_TARGET_NAME} SHARED)
    include(${ANDROID_SDK}/android_openssl/CMakeLists.txt)

    if (ANDROID_ABI STREQUAL "armeabi-v7a")
        set(KS_ANDROID_SSL_DIR "${SSL_ROOT_PATH}/latest/arm/")
    elseif (ANDROID_ABI STREQUAL "arm64-v8a")
        set(KS_ANDROID_SSL_DIR "${SSL_ROOT_PATH}/latest/arm64/")
    elseif (ANDROID_ABI STREQUAL "x86")
        set(KS_ANDROID_SSL_DIR "${SSL_ROOT_PATH}/latest/x86/")
    elseif (ANDROID_ABI STREQUAL "x86_64")
        set(KS_ANDROID_SSL_DIR "${SSL_ROOT_PATH}/latest/x86_64/")
    endif()

    message(STATUS "Android OpenSSL root path:    [${SSL_ROOT_PATH}]")
    message(STATUS "Android OpenSSL library path: [${KS_ANDROID_SSL_DIR}]")

# Linux
elseif(VS_PLATFORM STREQUAL "linux")
    add_executable(${KS_TARGET_NAME})
    # Add deployers targets
    include(deployers)

# MacOS
elseif(KS_PLATFORM STREQUAL "macos")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
    set(MACOSX_BUNDLE_ICON_FILE MyIcon.icns)
    set(MACOSX_DMG_ICON_FILE Installer.icns)
    set(MACOSX_BUNDLE_BACKGROUND_FILE "Background.png")
    set(MACOSX_DEPLOYMENT_TARGET 10.14)
    set(VS_BUNDLE_PREFIX "io.yiot.kutashenko")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "${VS_BUNDLE_PREFIX}.${PROJECT_NAME}")
    list(APPEND VS_TEMPLATES "${CMAKE_CURRENT_SOURCE_DIR}/platforms/macos/release-notes.html.in=${CMAKE_CURRENT_SOURCE_DIR}/platforms/macos/release-notes.html")
    list(APPEND VS_TEMPLATES "${CMAKE_CURRENT_SOURCE_DIR}/platforms/macos/dmg.json.in=${CMAKE_BINARY_DIR}/dmg.json")
    foreach(VS_TMP IN LISTS VS_TEMPLATES)
        string(REPLACE "=" ";" VS_TMP_LIST ${VS_TMP})
        list(GET VS_TMP_LIST 0 VS_TEMPLATE_SRC)
        list(GET VS_TMP_LIST 1 VS_TEMPLATE_DST)
        if(NOT EXISTS "${VS_TEMPLATE_SRC}")
            message(FATAL_ERROR "Template [${VS_TEMPLATE_SRC}] not found !")
        endif()
        message(STATUS "Processing template file: [${VS_TEMPLATE_SRC}]")
        configure_file("${VS_TEMPLATE_SRC}" "${VS_TEMPLATE_DST}" @ONLY)
    endforeach()

    include(deployers)

    set(App_ICON "${CMAKE_CURRENT_SOURCE_DIR}/platforms/macos/pkg_resources/${MACOSX_BUNDLE_ICON_FILE}")
    set_source_files_properties(${App_ICON}
            PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    # Sparkle
    #message(STATUS "Copy Sparkle framework")
    #execute_process(
    #        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/${KS_TARGET_NAME}.app/Contents/Frameworks
    #        COMMAND cp -Rf ${PREBUILT_DIR}/${KS_PLATFORM}/sparkle/Sparkle.framework ${CMAKE_BINARY_DIR}/${KS_TARGET_NAME}.app/Contents/Frameworks
    #)

    add_executable(${KS_TARGET_NAME} MACOSX_BUNDLE ${App_ICON} )

    set_target_properties(${KS_TARGET_NAME}
            PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_LIST_DIR}/platforms/macos/Info.plist.in"
            )
    #set_target_properties(${KS_TARGET_NAME}
    #        PROPERTIES LINK_FLAGS "-F${PREBUILT_DIR}/${KS_PLATFORM}/sparkle -framework Sparkle -framework CoreFoundation -framework Foundation"
    #        )
    #    macdeployqt(${KS_TARGET_NAME} "${CMAKE_CURRENT_LIST_DIR}/src/qml")

# Other
else()
    add_executable(${KS_TARGET_NAME})
endif()

# ---------------------------------------------------------------------------
#   IoTKit Qt Wrapper
# ---------------------------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/cmake/iotkit-qt)

# ---------------------------------------------------------------------------
#   Qt Keychain
# ---------------------------------------------------------------------------
set(BUILD_TEST_APPLICATION CACHE BOOL OFF FORCE)
set(BUILD_TRANSLATIONS CACHE BOOL OFF FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../ext/qtkeychain qtkeychain)

# ---------------------------------------------------------------------------
#	Header/Source files
# ---------------------------------------------------------------------------
target_sources(${KS_TARGET_NAME}
        PRIVATE

        # Headers
        ${CMAKE_CURRENT_LIST_DIR}/include/KSQApplication.h
        ${CMAKE_CURRENT_LIST_DIR}/include/ui/VSQUiHelper.h
        ${CMAKE_CURRENT_LIST_DIR}/include/KSQWiFiEnumerator.h
        ${CMAKE_CURRENT_LIST_DIR}/include/KSQBLEController.h

        ${CMAKE_CURRENT_LIST_DIR}/include/devices/KSQDevices.h
        ${CMAKE_CURRENT_LIST_DIR}/include/devices/KSQControllerBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/devices/KSQDeviceBase.h

        ${CMAKE_CURRENT_LIST_DIR}/include/devices/lamp/KSQLamp.h
        ${CMAKE_CURRENT_LIST_DIR}/include/devices/lamp/KSQLampController.h
        ${CMAKE_CURRENT_LIST_DIR}/include/devices/pc/KSQPC.h
        ${CMAKE_CURRENT_LIST_DIR}/include/devices/pc/KSQPCController.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/KSQIoTKitFacade.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/netif/KSQUdp.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/snap/KSQSnapLampClient.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/snap/KSQSnapPCClient.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/snap/KSQSnapPRVSClient.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/snap/KSQSnapSCRTClient.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/root-of-trust/KSQRoT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/root-of-trust/KSQRoTController.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/root-of-trust/KSQTrustList.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secmodule/KSQSecModule.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secmodule/KSQPublicKey.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secmodule/KSQPrivateKey.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secmodule/KSQSignature.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/storages/KSQStorageBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/storages/KSQStorageKeychain.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/storages/KSQStorageFS.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secbox/KSQSecBox.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/provision/KSQProvision.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/setup/KSQDeviceSetupController.h

        $<$<STREQUAL:"${KS_PLATFORM}","macos">:${CMAKE_CURRENT_LIST_DIR}/include/KSWiFi.h>
        $<$<STREQUAL:"${KS_PLATFORM}","windows">:${CMAKE_CURRENT_LIST_DIR}/include/KSWiFi.h>
        $<$<STREQUAL:"${KS_PLATFORM}","android">:${CMAKE_CURRENT_LIST_DIR}/include/android/KSQAndroid.h>

        # Sources
        ${CMAKE_CURRENT_LIST_DIR}/src/KSQApplication.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/ui/VSQUiHelper.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/KSQWiFiEnumerator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/KSQBLEController.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/iotkit-hal.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/devices/KSQDevices.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/devices/KSQControllerBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/devices/KSQDeviceBase.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/devices/lamp/KSQLamp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/devices/lamp/KSQLampController.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/devices/pc/KSQPC.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/devices/pc/KSQPCController.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/KSQIoTKitFacade.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/netif/KSQUdp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/snap/KSQSnapLampClient.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/snap/KSQSnapPCClient.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/snap/KSQSnapPRVSClient.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/snap/KSQSnapSCRTClient.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/root-of-trust/KSQRoT.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/root-of-trust/KSQRoTController.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/root-of-trust/KSQTrustList.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secmodule/KSQSecModule.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secmodule/KSQPublicKey.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secmodule/KSQPrivateKey.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secmodule/KSQSignature.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/storages/KSQStorageBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/storages/KSQStorageKeychain.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/storages/KSQStorageFS.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secbox/KSQSecBox.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/provision/KSQProvision.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/setup/KSQDeviceSetupController.cpp

        $<$<STREQUAL:"${KS_PLATFORM}","macos">:${CMAKE_CURRENT_LIST_DIR}/src/macos/KSMacWiFi.mm>
        $<$<STREQUAL:"${KS_PLATFORM}","windows">:${CMAKE_CURRENT_LIST_DIR}/src/win/KSWinWiFi.cpp>
        $<$<STREQUAL:"${KS_PLATFORM}","android">:${CMAKE_CURRENT_LIST_DIR}/src/android/KSQAndroid.cpp>

        # Qt Resources
        ${CMAKE_CURRENT_LIST_DIR}/src/resources.qrc
        )


# ---------------------------------------------------------------------------
#	Definitions
# ---------------------------------------------------------------------------
target_compile_definitions(${KS_TARGET_NAME}
        PRIVATE
        -DQKEYCHAIN_STATICLIB
        -DKS_TARGET_VERSION=${KS_TARGET_VERSION}
        -DKS_BUILD_NUMBER=${KS_BUILD_NUMBER}
        )


# ---------------------------------------------------------------------------
#   Include directories
# ---------------------------------------------------------------------------
target_include_directories(${KS_TARGET_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../ext/qtkeychain>
        )


# ---------------------------------------------------------------------------
#	Compile options
# ---------------------------------------------------------------------------
target_compile_options(${KS_TARGET_NAME}
        PRIVATE
        )


# ---------------------------------------------------------------------------
#	Link libraries
# ---------------------------------------------------------------------------
target_link_libraries(${KS_TARGET_NAME}
        PUBLIC

        # Qt5
        Qt5::Core
        Qt5::Widgets
        Qt5::Test
        Qt5::Network
        Qt5::Concurrent
        Qt5::Qml

        #   IoTKit Qt
        iotkit-qt-lib
        yiot-qos1-helper
        vs-default-soft-secmodule
        vs-module-secbox
        converters

        #   Qt keychain
        qtkeychain

        #   Windows
        $<$<STREQUAL:"${KS_PLATFORM}","windows">:wlanapi>

        #   Linux
        $<$<STREQUAL:"${KS_PLATFORM}","linux">:pthread>

        #   Android
        $<$<STREQUAL:"${KS_PLATFORM}","android">:Qt5::AndroidExtras>

        # Compiler options
        enable_pedantic_mode
        )

if (KS_PLATFORM STREQUAL "macos")
    #   macOS
    target_link_libraries(${KS_TARGET_NAME}
            PUBLIC
            "-framework CoreWLAN"
            "-framework Foundation"
            )

endif()

#------------------------------------------------------------------------------
#   Format code
# ---------------------------------------------------------------------------
if (COMMAND add_clangformat)
    add_clangformat(${KS_TARGET_NAME})
endif ()


#------------------------------------------------------------------------------
#   Install
# ---------------------------------------------------------------------------
install(TARGETS ${KS_TARGET_NAME}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        )


#------------------------------------------------------------------------------
#   Deployment
# ---------------------------------------------------------------------------
if (KS_PLATFORM STREQUAL "macos")
   #
   #   Configure plist file
   #
   set_target_properties(${KS_TARGET_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/platforms/macos/macos-info.plist)
   set (MACOSX_DEPLOYMENT_TARGET 10.14)
   set (PLIST_DST "${CMAKE_CURRENT_LIST_DIR}/platforms/macos/macos-info.plist")
   set (PLIST_TMPL "${PLIST_DST}.in")
   configure_file(${PLIST_TMPL} ${PLIST_DST})

#   macdeployqt(${KS_TARGET_NAME} "${CMAKE_CURRENT_LIST_DIR}/src/qml")
endif ()

