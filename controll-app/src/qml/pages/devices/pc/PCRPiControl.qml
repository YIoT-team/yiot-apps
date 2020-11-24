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
import "../../../components/validators"

Page {
    property var controller
    property alias deviceName: header.title

    id: p

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        id: header
        title: qsTr("RPi Controll")
        backAction: function() { showDevices() }
    }

    Form {
        id: form
        stretched: true

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: 40
            Layout.bottomMargin: 20

            spacing: 10

            Image {
                id: stateImage

                Layout.preferredHeight: 96
                Layout.fillWidth: true

                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/icons/dark/devices/pc/rpi.png"
            }

            InputTextField {
                id: userName
                label: qsTr("User name")
                placeholderText: qsTr("Enter new User name")
                validator: ValidatorUserName {}
            }

            Password {
                id: pass1
                label: qsTr("Password")
                placeholderText: qsTr("Enter new User password")
            }

            Password {
                id: pass2
                label: qsTr("Password check")
                placeholderText: qsTr("Enter new User password to check")
            }

            InputTextField {
                id: staticIP
                label: qsTr("Static IP")
                placeholderText: qsTr("Enter Static IP for RPi")
                validator: ValidatorIPv4 {}
            }

            FormSecondaryButton {
                Layout.topMargin: 20
                Layout.bottomMargin: 10
                text: qsTr("Init RPi")
                onClicked: {
                    if (validateInputs()) {
                        showCmdProcessing()
                        controller.initDevice(userName.text, pass1.text, staticIP.text)
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }

            FormPrimaryButton {
                Layout.bottomMargin: 10
                text: qsTr("Close")
                onClicked: {
                    showDevices()
                }
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

        if (staticIP.text == "") {
            showPopupError(qsTr("Static IP address isn't valid"), errorPopupClick)
            return false
        }

        return true
    }

}
