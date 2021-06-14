//  ────────────────────────────────────────────────────────────
//                     ╔╗  ╔╗ ╔══╗      ╔════╗
//                     ║╚╗╔╝║ ╚╣╠╝      ║╔╗╔╗║
//                     ╚╗╚╝╔╝  ║║  ╔══╗ ╚╝║║╚╝
//                      ╚╗╔╝   ║║  ║╔╗║   ║║
//                       ║║   ╔╣╠╗ ║╚╝║   ║║
//                       ╚╝   ╚══╝ ╚══╝   ╚╝
//    ╔╗╔═╗                    ╔╗                     ╔╗
//    ║║║╔╝                   ╔╝╚╗                    ║║
//    ║╚╝╝  ╔══╗ ╔══╗ ╔══╗  ╔╗╚╗╔╝  ╔══╗ ╔╗ ╔╗╔╗ ╔══╗ ║║  ╔══╗
//    ║╔╗║  ║║═╣ ║║═╣ ║╔╗║  ╠╣ ║║   ║ ═╣ ╠╣ ║╚╝║ ║╔╗║ ║║  ║║═╣
//    ║║║╚╗ ║║═╣ ║║═╣ ║╚╝║  ║║ ║╚╗  ╠═ ║ ║║ ║║║║ ║╚╝║ ║╚╗ ║║═╣
//    ╚╝╚═╝ ╚══╝ ╚══╝ ║╔═╝  ╚╝ ╚═╝  ╚══╝ ╚╝ ╚╩╩╝ ║╔═╝ ╚═╝ ╚══╝
//                    ║║                         ║║
//                    ╚╝                         ╚╝
//
//    Lead Maintainer: Roman Kutashenko <kutashenko@gmail.com>
//  ────────────────────────────────────────────────────────────

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../theme"
import "../components"
import "../components/devices"

Page {

    property var lastActiveDevice: null

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Controlled devices")
        showBackButton: false
        showMenuButton: true
        showSettingsButton: true
    }

    DeviceInfoPopup {
        id: deviceInfo
    }

    AllDevicesList {
        id: list
        model: deviceControllers
    }

    // Fill data and show device info.
    function showDeviceInfo(model) {
        deviceInfo.mName = model.name
        deviceInfo.mAddr = model.macAddr
        deviceInfo.mID = model.deviceID
        deviceInfo.mManufacturer = model.manufacture
        deviceInfo.mType = model.deviceType
        deviceInfo.mRoles = model.roles
        deviceInfo.mFwVer = model.fwVersion
        deviceInfo.mTlVer = model.tlVersion
        deviceInfo.mSent = model.sentBytes
        deviceInfo.mReceived = model.receivedBytes
        deviceInfo.open()
    }

    //  Choose controls view for Category element.
    function deviceCategoryActions(deviceCategory) {
        if (deviceCategory === "lamps") {
            return "qrc:/qml/components/devices/GeneralCategoryControls.qml"
        }  else if (deviceCategory === "pc") {
            return "qrc:/qml/components/devices/GeneralCategoryControls.qml"
        }

        console.error("Unknown Device Category : ", deviceCategory)
        return ""
    }

    //  Choose controls view per Device, by its type. Small controls in all-devices view.
    function deviceActions(deviceType) {
        if (deviceType === "lampMono") {
            return "qrc:/qml/components/devices/lamp/LampControls.qml"
        } else if (deviceType === "pc") {
            return "qrc:/qml/components/devices/GeneralDeviceControls.qml"
        }

        console.error("Small controls: Unknown Device Type")
        return ""
    }

    // Show Big Controls view for device, by its type.
    function activateDeviceView(deviceType, deviceName, deviceController) {
        lastActiveDevice = deviceController
        if (deviceType === "lampMono") {
            showLampMono(deviceName, deviceController)
            return
        } else if (deviceType === "pc") {
            showPC(deviceName, deviceController)
            return
        }

        console.error("Control page: Unknown Device Type")
    }

    // Device state image. Left side icon in all-devices view, per each device.
    function deviceStateImage(model) {
        if (model.deviceType === "lampMono") {
            return "devices/lamp/mono/%1".arg(model.state)
        } else if (model.deviceType === "pc") {
            return "devices/pc/rpi"
        }

        console.error("State image: Unknown Device Type")
        return ""
    }
}
