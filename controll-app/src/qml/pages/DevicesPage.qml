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

    // Show Big Controls view for device, by its type.
    function activateDeviceView(deviceController) {
        lastActiveDevice = deviceController
        showDeviceControl(deviceController)
    }
}
