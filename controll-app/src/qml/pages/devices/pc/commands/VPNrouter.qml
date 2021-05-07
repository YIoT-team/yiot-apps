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
import "../../../../components/validators"
import "../../../../../js/devices/pc.js" as PCDevice

Page {
    id: vpnRouterPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("VPN Router")
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
                text: "Select VPN provider:"
                Layout.leftMargin: 31
                Layout.bottomMargin: 0
            }

            FormComboBox {
                id: providerCb
                Layout.leftMargin: 12
                Layout.topMargin: 0
                items: ["OVPN"]
            }

            InputTextField {
                id: user
                label: qsTr("User")
                placeholderText: qsTr("Enter user name")
                validator: ValidatorUserName {}
            }

            Password {
                id: pass
                label: qsTr("Password")
                placeholderText: qsTr("Enter user password")
                validator: ValidatorPassword {}
            }

            FormSecondaryButton {
                Layout.topMargin: 20
                Layout.bottomMargin: 10
                text: qsTr("Save")
                onClicked: {
                    if(validateInputs()){
                        showCmdProcessing(rpiPage.controller)
                        PCDevice.setupVPNRouter(rpiPage.controller,
                                                providerCb.text,
                                                user.text,
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
        if (user.text == "") {
            showPopupError(qsTr("Enter user name"), errorPopupClick)
            return false
        }

        if (pass.text == "") {
            showPopupError(qsTr("Enter user password"), errorPopupClick)
            return false
        }

        return true
    }
}
