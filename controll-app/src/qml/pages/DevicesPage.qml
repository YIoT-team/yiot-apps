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
        visible: !deviceControllers.empty
        model: deviceControllers
    }

    DevicesDetectionProgress {
        visible: deviceControllers.empty && !busyDialog.visible
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
                applicationWindow.setAppBusy(true)
                app.updateDevices()
                delayed(5000, function() {
                    enabled = true
                    applicationWindow.setAppBusy(false)
                })
            }

            function delayed(delayTime, cb) {
                timer.interval = delayTime;
                timer.repeat = false;
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
