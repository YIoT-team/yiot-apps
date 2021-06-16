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

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include(GNUInstallDirs)
include(FetchContent)

# ---------------------------------------------------------------------------
#   Poject Name
# ---------------------------------------------------------------------------
set (KS_TARGET_NAME yiot)

set(VS_ORGANIZATION_NAME "YIoT")
set(VS_ORGANIZATION_DISPLAYNAME "YIoT")

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
    PAD_STRING(ANDROID_VERSION_CODE 3 "0" "${TMP_VERS}")

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

    # Android extra libraries
    if(ANDROID_BUILD_ABI_arm64-v8a)
        list(APPEND ANDROID_EXTRA_LIBS ${CMAKE_BINARY_DIR}/common/iotkit/modules/crypto/converters/libconverters-arm64-v8a.so)
    endif()

    if(ANDROID_BUILD_ABI_armeabi-v7a)
        list(APPEND ANDROID_EXTRA_LIBS ${CMAKE_BINARY_DIR}/common/iotkit/modules/crypto/converters/libconverters-armeabi-v7a.so)
    endif()

    if(ANDROID_BUILD_ABI_x86_64)
        list(APPEND ANDROID_EXTRA_LIBS ${CMAKE_BINARY_DIR}/common/iotkit/modules/crypto/converters/libconverters-x86_64.so)
    endif()

    if(ANDROID_BUILD_ABI_x86)
        list(APPEND ANDROID_EXTRA_LIBS ${CMAKE_BINARY_DIR}/common/iotkit/modules/crypto/converters/libconverters-x86.so)
    endif()

    # Android gradle path
    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/platforms/android")
endif()

# ---------------------------------------------------------------------------
#   Qt
# ---------------------------------------------------------------------------
find_package(Qt5 COMPONENTS Core Widgets Test Network Concurrent WebSockets Qml ${KS_QT_EXTRA} REQUIRED)

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
elseif(KS_PLATFORM STREQUAL "linux")
    add_executable(${KS_TARGET_NAME})
    # Add deployers targets
    include(deployers)
    
# Windows
elseif(KS_PLATFORM STREQUAL "windows")
    add_executable(${KS_TARGET_NAME})
    include(deployers)    
    configure_file( "${PROJECT_SOURCE_DIR}/controll-app/platforms/windows/yiot.nsi.in" "${CMAKE_BINARY_DIR}/yiot.nsi" "${VS_TEMPLATE_DST}" @ONLY)

# MacOS
elseif(KS_PLATFORM STREQUAL "macos")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
    set(MACOSX_BUNDLE_ICON_FILE MyIcon.icns)
    set(MACOSX_DMG_ICON_FILE Installer.icns)
    set(MACOSX_BUNDLE_BACKGROUND_FILE "Background.png")
    set(MACOSX_DEPLOYMENT_TARGET 10.13)
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
set(QTANDROIDEXTRAS_LIBRARIES "Qt5::AndroidExtras")
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

        ${CMAKE_CURRENT_LIST_DIR}/include/controllers/KSQDevice.h
        ${CMAKE_CURRENT_LIST_DIR}/include/controllers/KSQDevicesType.h
        ${CMAKE_CURRENT_LIST_DIR}/include/controllers/KSQAllDevicesController.h
        ${CMAKE_CURRENT_LIST_DIR}/include/controllers/KSQBlankDevicesController.h
        ${CMAKE_CURRENT_LIST_DIR}/include/controllers/KSQBLEController.h
        ${CMAKE_CURRENT_LIST_DIR}/include/controllers/KSQUXSimplifyController.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/KSQIoTKitFacade.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/netif/KSQUdp.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/netif/KSQNetifWebsocket.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/snap/KSQSnapUSERClient.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/snap/KSQSnapPRVSClient.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/snap/KSQSnapSCRTClient.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/root-of-trust/KSQRoT.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/root-of-trust/KSQRoTController.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/root-of-trust/KSQTrustList.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secmodule/KSQSecModule.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secmodule/KSQPublicKey.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secmodule/KSQPrivateKey.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secmodule/KSQSessionKey.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secmodule/KSQSignature.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/storages/KSQStorageBase.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/storages/KSQStorageKeychain.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/storages/KSQStorageFS.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/secbox/KSQSecBox.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/provision/KSQProvision.h

        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/setup/KSQDeviceSetupController.h
        ${CMAKE_CURRENT_LIST_DIR}/include/yiot-iotkit/setup/KSQDeviceSetupData.h

        $<$<STREQUAL:"${KS_PLATFORM}","macos">:${CMAKE_CURRENT_LIST_DIR}/include/KSWiFi.h>
        $<$<STREQUAL:"${KS_PLATFORM}","windows">:${CMAKE_CURRENT_LIST_DIR}/include/KSWiFi.h>
        $<$<STREQUAL:"${KS_PLATFORM}","linux">:${CMAKE_CURRENT_LIST_DIR}/include/KSWiFi.h>
        $<$<STREQUAL:"${KS_PLATFORM}","android">:${CMAKE_CURRENT_LIST_DIR}/include/os/android/KSQAndroid.h>

        # Sources
        ${CMAKE_CURRENT_LIST_DIR}/src/KSQApplication.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/ui/VSQUiHelper.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/KSQWiFiEnumerator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/iotkit-hal.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/controllers/KSQDevice.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/controllers/KSQDevicesType.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/controllers/KSQAllDevicesController.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/controllers/KSQBlankDevicesController.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/controllers/KSQBLEController.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/controllers/KSQUXSimplifyController.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/KSQIoTKitFacade.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/netif/KSQUdp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/netif/KSQNetifWebsocket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/snap/KSQSnapUSERClient.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/snap/KSQSnapPRVSClient.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/snap/KSQSnapSCRTClient.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/root-of-trust/KSQRoT.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/root-of-trust/KSQRoTController.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/root-of-trust/KSQTrustList.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secmodule/KSQSecModule.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secmodule/KSQPublicKey.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secmodule/KSQPrivateKey.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secmodule/KSQSessionKey.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secmodule/KSQSignature.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/storages/KSQStorageBase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/storages/KSQStorageKeychain.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/storages/KSQStorageFS.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/secbox/KSQSecBox.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/provision/KSQProvision.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/setup/KSQDeviceSetupController.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/yiot-iotkit/setup/KSQDeviceSetupData.cpp

        $<$<STREQUAL:"${KS_PLATFORM}","macos">:${CMAKE_CURRENT_LIST_DIR}/src/os/macos/KSMacWiFi.mm>
        $<$<STREQUAL:"${KS_PLATFORM}","linux">:${CMAKE_CURRENT_LIST_DIR}/src/os/linux/KSLinuxWiFi.cpp>
        $<$<STREQUAL:"${KS_PLATFORM}","windows">:${CMAKE_CURRENT_LIST_DIR}/src/os/win/KSWinWiFi.cpp>
        $<$<STREQUAL:"${KS_PLATFORM}","android">:${CMAKE_CURRENT_LIST_DIR}/src/os/android/KSQAndroid.cpp>

        # Qt Resources
        ${CMAKE_CURRENT_LIST_DIR}/src/resources.qrc
        ${CMAKE_CURRENT_LIST_DIR}/extensions/devices/rpi/device.qrc
        ${CMAKE_CURRENT_LIST_DIR}/extensions/integrations/websocket/integration.qrc
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
        $<$<STREQUAL:"${KS_PLATFORM}","android">:Qt5::AndroidExtras>
        Qt5::Widgets
        Qt5::Test
        Qt5::Network
        Qt5::WebSockets
        Qt5::Concurrent
        Qt5::Qml

        #   IoTKit Qt
        iotkit-qt-lib
        yiot-qos1-helper
        vs-default-soft-secmodule
        vs-module-secbox

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
   set (PLIST_DST "${CMAKE_CURRENT_LIST_DIR}/platforms/macos/macos-info.plist")
   set (PLIST_TMPL "${PLIST_DST}.in")
   configure_file(${PLIST_TMPL} ${PLIST_DST})

#   macdeployqt(${KS_TARGET_NAME} "${CMAKE_CURRENT_LIST_DIR}/src/qml")
endif ()

