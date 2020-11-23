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

    function deviceCategoryActions(deviceType) {
        if (deviceType === "lamps") {
            return "qrc:/qml/components/devices/GeneralCategoryControls.qml"
        }  else if (deviceType === "pc") {
            return "qrc:/qml/components/devices/GeneralCategoryControls.qml"
        }

        console.error("Unknown Device Type")
        return ""
    }

    function deviceActions(deviceType) {
        if (deviceType === "lampMono") {
            return "qrc:/qml/components/devices/lamp/LampControls.qml"
        } else if (deviceType === "pc") {
            return "qrc:/qml/components/devices/GeneralDeviceControls.qml"
        }

        console.error("Unknown Device Type")
        return ""
    }

    function activateDeviceView(deviceType, deviceName, deviceController) {
        if (deviceType === "lampMono") {
            lampMonoPage.deviceName = deviceName
            lampMonoPage.controller = deviceController
            showLampMono()
        } else if (deviceType === "pc") {
            showPC()
        }

        console.error("Unknown Device Type")
    }

    function deviceStateImage(model) {
        if (model.deviceType === "lampMono") {
            return "devices/lamp/mono/%1".arg(model.state)
        } else if (model.deviceType === "pc") {
            return "devices/pc/rpi"
        }

        console.error("Unknown Device Type")
        return ""
    }
}
