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
import "../../components"

Page {
    id: accountSettingsPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Account settings")
        backAction: function() { showMenuSettings() }
    }

    ColumnLayout {
        width: parent.width
        anchors.topMargin: 30
        spacing: 20

        TextField {
            id: login
            placeholderText: qsTr("Login here")
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter
            color: Theme.primaryTextColor
            font.pointSize: 14
            leftPadding: 30
            background: Rectangle {
                implicitWidth: 200
                implicitHeight: 50
                radius: implicitHeight / 2
                color: "transparent"

                Rectangle {
                    width: parent.width - 10
                    height: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    color: Theme.brandColor
                }
            }
        }

        TextField {
            id: pass
            placeholderText: qsTr("Pass here")
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter
            color: Theme.primaryTextColor
            font.pointSize: 14
            leftPadding: 30
            echoMode: TextField.PasswordEchoOnEdit
            background: Rectangle {
                implicitWidth: 200
                implicitHeight: 50
                radius: implicitHeight / 2
                color: "transparent"

                Rectangle {
                    width: parent.width - 10
                    height: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    color: Theme.brandColor
                }
            }
        }

        FormPrimaryButton {
            Layout.bottomMargin: 10
            text: qsTr("Apply credentials")
            onClicked: {
                if(validateInputs()) {
                    settings.setServicesLogin(login.text)
                    settings.setServicesPass(pass.text)
                    showMenuSettings()
                }
            }
        }
    }

    onVisibleChanged: {
        login.text = settings.getServicesLogin()
        pass.text = settings.getServicesPass ()
    }

    function errorPopupClick() {}

    function validateInputs() {
        if (login.text == "") {
            showPopupError(qsTr("Set login"), errorPopupClick)
            return false
        }

        if (pass.text == "") {
            showPopupError(qsTr("Set password"), errorPopupClick)
            return false
        }

        return true
    }
}
