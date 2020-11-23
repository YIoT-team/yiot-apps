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
    property string ssid: ""

    id: wifiSettingsPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        id: header
        title: qsTr("Set WiFi password ") + ssid
        backAction: function() { showWiFiSettings() }
    }

    ColumnLayout {
        width: parent.width
        anchors.topMargin: 30
        spacing: 20

        TextField {
            id: password
            placeholderText: qsTr("Password Hint")
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter
            color: Theme.primaryTextColor
            font.pointSize: UiHelper.fixFontSz(14)
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
            text: qsTr("Apply password")
            onClicked: {
                useWiFiNetwork(ssid, password.text)
            }
        }
    }

    function prepare(aSsid) {
        ssid = aSsid;

        // Set page title
        header.title = qsTr("WiFi password for ") + ssid

        // Load data from KeyChain
        settings.loadWiFiCred(ssid, function (s, p) {
            password.text = p
            console.log("Password is: " + p)
        })
    }

    function useWiFiNetwork(ssid, pass) {
        // Set default WiFi Network in settings
        settings.setWiFiCredDefault(ssid, pass)

        // Switch to devices page
        showDevicesSetup()
    }
}
