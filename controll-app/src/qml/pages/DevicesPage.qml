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

import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 1.15

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

    footer: ColumnLayout {
        width: parent.width
        FormSecondaryButton {
            Layout.topMargin: 20
            Layout.bottomMargin: 10
            text: qsTr("Refresh")

            Timer {
                id: timer
            }

            onClicked: {
                enabled = false
                app.updateDevices()
                delayed(5000, function() {
                    enabled = true
                })
            }

            function delayed(delayTime, cb) {
                timer.interval = delayTime;
                timer.repeat = true;
                timer.triggered.connect(cb);
                timer.start();
            }
        }
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
