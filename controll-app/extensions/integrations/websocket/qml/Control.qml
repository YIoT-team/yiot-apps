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

import "qrc:/qml/theme"
import "qrc:/qml/components"
import "qrc:/qml/components/validators"

Page {
    id: websocketPage
    anchors.fill: parent

    background: Rectangle {
        color: "transparent"
    }

//    header: Header {
//        title: qsTr("Websocket")
//        backAction: function() { /* todo */ }
//    }

    Form {
            id: form
            stretched: true

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.topMargin: 40
                Layout.bottomMargin: 20

                spacing: 15

                InputTextField {
                    id: link
                    label: qsTr("Websocket Link")
                    placeholderText: qsTr("Enter websocket link")
                    text: settings.getWebsocketLink()
                    //validator: todo
                }

                FormPrimaryButton {
                    Layout.topMargin: 20
                    Layout.bottomMargin: 10
                    visible: !settings.getWebsocketState()

                    text: qsTr("Enable")
                    onClicked: {
                        settings.setWebsocketState(true)
                        settings.setWebsocketLink(link.text)
                        console.log("WebSocket Link: " + settings.getWebsocketLink())
                    }
                }

                FormPrimaryButton {
                    Layout.topMargin: 20
                    Layout.bottomMargin: 10
                    visible: settings.getWebsocketState()

                    text: qsTr("Disable")
                    onClicked: {
                        settings.setWebsocketState(false)
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
        }
}
