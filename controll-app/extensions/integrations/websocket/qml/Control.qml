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
    property var controller
    property bool isEnabled

    id: websocketPage
    visible: false

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Websocket")
        backAction: function() { showIntegrationsList() }
    }

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
                text: ""
            }

            FormPrimaryButton {
                Layout.topMargin: 20
                Layout.bottomMargin: 10
                visible: !websocketPage.isEnabled

                text: qsTr("Enable")
                onClicked: {
                    websocketPage.isEnabled = true
                    controller.js.activate(link.text)
                }
            }

            FormPrimaryButton {
                Layout.topMargin: 20
                Layout.bottomMargin: 10
                visible: websocketPage.isEnabled

                text: qsTr("Disable")
                onClicked: {
                    websocketPage.isEnabled = false
                    controller.js.deactivate()
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
    onVisibleChanged: {
        if (visible) {
            link.text = controller.js.getLink()
            websocketPage.isEnabled = controller.js.integrationState()
        }
    }
}
