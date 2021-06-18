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

import QtQuick 2.5
import QtQuick.Layouts 1.5
import QtQuick.Controls 2.12

import "../theme"

ListView {
    id: wifiList

    property string ssid: ""

    Layout.fillHeight: true
    Layout.fillWidth: true

    spacing: 1
    model: wifiEnum

    delegate: Rectangle {
        id: base
        width: wifiList.width
        height: 45
        color: colorIfActive(name)

        RowLayout {
            id: listDelegate
            anchors.fill: parent
            clip: true

            Image {
                id: icon
                source: Theme.wifiImg
                Layout.maximumHeight: listDelegate.height * 0.6
                Layout.maximumWidth: Layout.maximumHeight
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 5
            }

            Text {
                id: nameText
                text: name
                color: Theme.primaryTextColor
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 14

                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Text {
                id: rssiText
                text: rssi
                color: Theme.primaryTextColor
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 14

                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true
                Layout.rightMargin: 15
            }
        }

        MouseArea {
            enabled: true
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                wifiList.currentIndex = index
                ssid = name
                showWiFiPassPage(ssid)
            }

            onEntered: {
                wifiList.currentIndex = index
                base.color = Theme.contrastBackgroundColor
            }

            onExited: {
                base.color = colorIfActive(name)
            }
        }
    }

    function colorIfActive(name) {
        var isActive = settings.defaultWiFi === name
        return isActive ? Theme.contrastBackgroundColor : "transparent"
    }
}
