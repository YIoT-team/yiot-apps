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

import "qrc:/qml/components"
import "qrc:/qml/components/validators"
//import "../../../../../js/devices/main.qml" as PCDevice

Page {
    id: createUserPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Create User")
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
                    id: userName
                    label: qsTr("User name")
                    placeholderText: qsTr("Enter new user name")
                    validator: ValidatorUserName {}
                }

                Password {
                    id: pass1
                    label: qsTr("Password")
                    placeholderText: qsTr("Enter new password")
                    validator: ValidatorPassword {}
                }

                Password {
                    id: pass2
                    label: qsTr("Password check")
                    placeholderText: qsTr("Enter the password again")
                    validator: ValidatorPassword {}
                }

                FormSecondaryButton {
                    Layout.topMargin: 20
                    Layout.bottomMargin: 10
                    text: qsTr("Save")
                    onClicked: {
                        if(validateInputs()) {
                            showCmdProcessing(rpiPage.controller)
//                            PCDevice.createUser(rpiPage.controller, userName.text, pass1.text)
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }

    function errorPopupClick() {
        }

        function validateInputs() {
            if (userName.text == "") {
                showPopupError(qsTr("Set new User name"), errorPopupClick)
                return false
            }

            if (pass1.text == "") {
                showPopupError(qsTr("Set new User password"), errorPopupClick)
                return false
            }

            if (pass1.text != pass2.text) {
                showPopupError(qsTr("Passwords are not equal"), errorPopupClick)
                return false
            }

            return true
        }
}
