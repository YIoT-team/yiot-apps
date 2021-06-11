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
import "../../../../components/validators"
import "../../../../../js/devices/pc.js" as PCDevice

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
                    validator: ValidatorSSID {}
                }

                FormLabel {
                    id: comboBoxLabel
                    text: "Select encryption mode:"
                    Layout.leftMargin: 31
                    Layout.bottomMargin: 0
                }

                FormComboBox {
                    id: mode
                    Layout.leftMargin: 12
                    Layout.topMargin: 0
                    items: ["WPA2", "WPA", "WEP"]
                }

                Password {
                    id: pass
                    label: qsTr("Password")
                    placeholderText: qsTr("Enter new password")
                    validator: ValidatorPassword {}
                }

                FormSecondaryButton {
                    Layout.topMargin: 20
                    Layout.bottomMargin: 10
                    text: qsTr("Save")
                    onClicked: {
                        if(validateInputs()) {
                            showCmdProcessing(rpiPage.controller)
                            PCDevice.setupAccessPoint(rpiPage.controller,
                                                      ssid.text,
                                                      mode.text,
                                                      pass.text)
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }

    function errorPopupClick() {
        }

        function validateInputs() {
            if (ssid.text == "") {
                showPopupError(qsTr("Set SSID"), errorPopupClick)
                return false
            }

            if (pass.text == "") {
                showPopupError(qsTr("Set new password"), errorPopupClick)
                return false
            }

            return true
        }
}
