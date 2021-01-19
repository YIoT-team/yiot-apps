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

import "../../../theme"
import "../../../components"

ListView {
    id: list
    Layout.fillHeight: true
    Layout.fillWidth: true

    spacing: 1
    model: localBlankDevicesController

    delegate: Rectangle {
        id: base
        width: list.width
        height: 45
        color: "transparent"

        RowLayout {
            id: listDelegate
            anchors.fill: parent
            clip: true

            Image {
                id: icon
                source: Theme.wifiImg
                Layout.maximumHeight: listDelegate.height * 0.7
                Layout.maximumWidth: Layout.maximumHeight
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                id: nameText
                text: "%1 : %2".arg(name).arg(mac)
                color: Theme.primaryTextColor
                verticalAlignment: Text.AlignVCenter
                font.pointSize: UiHelper.fixFontSz(14)

                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

        MouseArea {
            enabled: true
            anchors.fill: parent
            hoverEnabled: true
            anchors.rightMargin: 0
            onClicked: {
                list.currentIndex = index
                provisionLocalDevice(mac)
            }

            onEntered: {
                list.currentIndex = index
                base.color = Theme.contrastBackgroundColor
            }

            onExited: {
                base.color = "transparent"
            }
        }
    }

    function provisionLocalDevice(mac) {
        showCredLoad()
        localBlankDevicesController.provisionDevice(mac)
//        PCDevice.createUser(mac, "user123", "pass1")
//        PCDevice.setNetworkParams("02:02:02:02:02:02", "wifi", true, "192.168.0.105", "192.168.0.1", "8.8.8.8", "255.255.255.0")
    }
}
