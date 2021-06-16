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

import "../../theme"
import "../../components/devices"

Item {
    property bool controlPageOpen: true

    id: p

    Layout.topMargin: 1
    Layout.leftMargin: 10

    ListView {
        anchors.fill: parent
        Rectangle {
            id: base
            width: parent.width
            height: 55
            color: "transparent"

            RowLayout {
                id: listDelegate
                anchors.fill: parent
                clip: true

                Image {
                    id: icon
                    source: "qrc:/integration/0/icons/%1/websocket.png".arg(Theme.state)
                    Layout.maximumHeight: listDelegate.height * 0.7
                    Layout.maximumWidth: Layout.maximumHeight
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: 10
                }

                Text {
                    id: nameText
                    text: qsTr("Websocket")
                    color: Theme.primaryTextColor
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: UiHelper.fixFontSz(14)

                    Layout.alignment: Qt.AlignLeft
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.leftMargin: 30
                }
            }

            MouseArea {
                //enabled: true
                anchors.fill: parent
                hoverEnabled: true
                //anchors.rightMargin: 0
                onClicked: { p.controlPageOpen = false }

                onEntered: {
                    base.color = Theme.contrastBackgroundColor
                }

                onExited: {
                    base.color = "transparent"
                }
            }
        }
    }
}
