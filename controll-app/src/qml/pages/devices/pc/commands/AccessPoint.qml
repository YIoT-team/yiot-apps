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

import "../../../../theme"
import "../../../../components"

Page {
    id: accessPointPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Access Point")
        backAction: function() { showRPiSettings() }
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
                    id: ssid
                    label: qsTr("SSID")
                    placeholderText: qsTr("Enter access point name")
                }

                FormLabel {
                    id: comboBoxLabel
                    text: "Select encryption mode:"
                    Layout.leftMargin: 31
                    Layout.bottomMargin: 0
                }

                FormComboBox {
                    Layout.leftMargin: 12
                    Layout.topMargin: 0
                    items: ["Mode 1", "Mode 2", "Mode 3"]
                }

                Password {
                    id: pass
                    label: qsTr("Password")
                    placeholderText: qsTr("Enter new password")
                }

                FormSecondaryButton {
                    Layout.topMargin: 20
                    Layout.bottomMargin: 10
                    text: qsTr("Save")
                    onClicked: {
                                                //        PCDevice.setNetworkParams(rpiPage.controller.macAddr, "wifi", true, "192.168.0.105", "192.168.0.1", "8.8.8.8", "255.255.255.0")
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
}
