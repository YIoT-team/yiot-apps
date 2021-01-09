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
    Layout.bottomMargin: 1

    spacing: 1
    model: bleEnum

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
                source: Theme.btImg
                Layout.maximumHeight: listDelegate.height * 0.7
                Layout.maximumWidth: Layout.maximumHeight
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignLeft
            }

            Text {
                id: nameText
                text: name
                color: Theme.primaryTextColor
                verticalAlignment: Text.AlignVCenter
                font.pointSize: UiHelper.fixFontSz(14)

                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Text {
                id: rssiText
                text: rssi
                color: Theme.primaryTextColor
                verticalAlignment: Text.AlignVCenter
                font.pointSize: UiHelper.fixFontSz(14)

                Layout.alignment: Qt.AlignLeft
                Layout.fillHeight: true
                Layout.rightMargin: 15
            }
        }

        MouseArea {
            enabled: true
            anchors.fill: parent
            hoverEnabled: true
            anchors.rightMargin: 0
            onClicked: {
                list.currentIndex = index
                connectBLEDevice(name)
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

    Component.onCompleted: {
    }

    function connectBLEDevice(name) {
        showCredLoad()
        bleController.connectDevice(name)
    }
}
