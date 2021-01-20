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

import "../../../../components"
import "../../../../../js/devices/pc.js" as PCDevice

Page {
    id: staticIpPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Static IP")
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

                FormLabel {
                    id: comboBoxLabel
                    text: "Select network interface:"
                    Layout.leftMargin: 31
                    Layout.bottomMargin: 0
                }

                FormComboBox {
                    id: interfaceCb
                    Layout.leftMargin: 12
                    Layout.topMargin: 0
                    items: ["wifi", "eth0"]
                }

                InputTextField {
                    id: deviceIP
                    label: qsTr("Device IP")
                    placeholderText: qsTr("Enter device static IP addres")
                }

                InputTextField {
                    id: gatewayIP
                    label: qsTr("Gateway IP")
                    placeholderText: qsTr("Enter gateway IP addres")
                }

                InputTextField {
                    id: mask
                    label: qsTr("Mask")
                    placeholderText: qsTr("Enter network mask")
                    text: "255.255.255.0"
                }

                InputTextField {
                    id: dns
                    label: qsTr("DNS")
                    placeholderText: qsTr("Enter DNS")
                    text: "8.8.8.8"
                }

                FormSecondaryButton {
                    Layout.topMargin: 20
                    Layout.bottomMargin: 10
                    text: qsTr("Save")
                    onClicked: {
                        showCmdProcessing(rpiPage.controller)
                        PCDevice.setNetworkParams(rpiPage.controller,
                                                  interfaceCb.text,
                                                  "true", // Force static
                                                  deviceIP.text,
                                                  gatewayIP.text,
                                                  dns.text,
                                                  mask.text)
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
}
