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

            InputTextField {
                id: apName
                label: qsTr("Access Point name")
                placeholderText: qsTr("Enter AP name")
                validator: ValidatorUserName {}
            }

            Password {
                id: apPass
                label: qsTr("Access Point Password")
                placeholderText: qsTr("Enter AP password")
                validator: ValidatorPassword {}
            }

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
                label: qsTr("VPN User")
                placeholderText: qsTr("Enter user name")
                validator: ValidatorUserName {}
            }

            Password {
                id: pass
                label: qsTr("VPN Password")
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
                                                apName.text,
                                                apPass.text,
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
        if (apName.text == "") {
            showPopupError(qsTr("Enter Access Point name"), errorPopupClick)
            return false
        }

        if (apPass.text == "") {
            showPopupError(qsTr("Enter Access Point password"), errorPopupClick)
            return false
        }

        if (user.text == "") {
            showPopupError(qsTr("Enter VPN user name"), errorPopupClick)
            return false
        }

        if (pass.text == "") {
            showPopupError(qsTr("Enter VPN password"), errorPopupClick)
            return false
        }

        return true
    }
}
