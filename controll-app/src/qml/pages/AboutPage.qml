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

import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 1.15

import "../theme"
import "../components"

//Page {
//    id: p
//    anchors.fill: parent
//    state: "connect"

//    background: Rectangle {
//        color: "transparent"
//    }

//    header: Header {
//        title: qsTr("About YIoT")
//        showBackButton: true
//        showMenuButton: false
//        showSettingsButton: false
//        backAction: function() { hideAbout() }
//    }

//    Form {
//        stretched: true

//        FormVendor {}
//        FormDoers {}
//    }
//}

SwipeView {
    id: testSwipeView
    anchors.fill: parent

    interactive: false
    currentIndex: 0

    Page {
        id: loginPage

        background: Rectangle {
            color: "transparent"
        }

        header: Header {
            title: qsTr("Wireguard Test")
            showBackButton: true
            showMenuButton: false
            showSettingsButton: false
            backAction: function() { hideAbout() }
        }

        ColumnLayout {
            width: parent.width
            anchors.topMargin: 30
            spacing: 20

            Label {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: Theme.mainFont
                font.pointSize: 24
                color: Theme.brandColor

                text: qsTr("Login")
            }

            TextField {
                id: username
                placeholderText: qsTr("Username Hint")
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
                        width: parent.width - 20
                        height: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        color: Theme.brandColor
                    }
                }
            }

            TextField {
                id: password
                placeholderText: qsTr("Password Hint")
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
                        width: parent.width - 20
                        height: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        color: Theme.brandColor
                    }
                }
            }

            FormPrimaryButton {
                Layout.bottomMargin: 10
                text: qsTr("Sign in")
                onClicked: {
                    login(username.text, password.text)
                }
            }
        }
    }

    Page {
        id: regNewClientPage

        background: Rectangle {
            color: "transparent"
        }

        header: Header {
            title: qsTr("Wireguard Test")
            showBackButton: true
            showMenuButton: false
            showSettingsButton: false
            backAction: function() { hideAbout(); testSwipeView.currentIndex = 0 }
        }

        ColumnLayout {
            width: parent.width
            anchors.topMargin: 30
            spacing: 20

            Label {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: Theme.mainFont
                font.pointSize: 24
                color: Theme.brandColor

                text: qsTr("Register a new WireGuard client")
            }

            TextField {
                id: nameField
                placeholderText: qsTr("Name Hint")
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
                        width: parent.width - 20
                        height: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        color: Theme.brandColor
                    }
                }
            }

            TextField {
                id: emailField
                placeholderText: qsTr("Email Hint")
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
                        width: parent.width - 20
                        height: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        color: Theme.brandColor
                    }
                }
            }

            TextField {
                id: ipField
                placeholderText: qsTr("10.252.1.1/32")
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
                        width: parent.width - 20
                        height: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        color: Theme.brandColor
                    }
                }
            }

            FormPrimaryButton {
                Layout.bottomMargin: 10
                text: qsTr("Register")
                onClicked: {
                    if (nameField.text.length !== 0) {
                        regNewClient(nameField.text, emailField.text, ipField.text)
                    } else {
                        showPopupError("Please enter a name for the new client", {})
                    }
                }
            }
        }
    }

    function login(username, password) {
        var postData = JSON.stringify(
                        {
                            "username" : username,
                            "password" : password
                        }
                    )

        var xhr = new XMLHttpRequest()

        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                if (xhr.status === 200) {
                    console.log(xhr.responseText)

                    nameField.text = ""
                    emailField.text = ""
                    ipField.text = ""

                    testSwipeView.currentIndex = 1
                } else {
                    console.error("Login request failed: " + xhr.status)

                    showPopupError("Login error: " + JSON.parse(xhr.responseText).message, {})
                }
            }
        }

        xhr.open("POST", "http://0.0.0.0:5000/login?next=/")
        xhr.setRequestHeader("Content-type", "application/json")
        xhr.setRequestHeader("Content-length", postData.length)
        xhr.send(postData)
    }

    function regNewClient(name, email, ip) {
        var postData = JSON.stringify(
                        {
                            "id" : generateRandomID(),
                            "private_key" : generateRandomKey(),
                            "public_key": generateRandomKey(),
                            "preshared_key" : generateRandomKey(),
                            "name" : name,
                            "email" : email,
                            "allocated_ips" : [ip],
                            "allowed_ips" : ["0.0.0.0/0"],
                            "extra_allowed_ips" : [],
                            "use_server_dns" : true,
                            "enabled" : true,
                            "created_at" : "2023-12-21T22:11:14.49461Z",
                            "updated_at" : "2023-12-21T22:11:14.49461Z"
                        }
                    )

        var xhr = new XMLHttpRequest()

        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                if (xhr.status === 200) {
                    console.log(xhr.responseText)

                    showPopupInform("New user successfully registered", function() {})

                    nameField.text = ""
                    emailField.text = ""
                    ipField.text = ""
                } else {
                    console.error("Registration request failed: " + xhr.status)

                    showPopupError("Registration error: " + JSON.parse(xhr.responseText).message, {})
                }
            }
        }

        xhr.open("POST", "http://0.0.0.0:5000/new-client")
        xhr.setRequestHeader("Content-type", "application/json")
        xhr.setRequestHeader("Content-length", postData.length)
        xhr.send(postData)
    }

    function generateRandomID() {
        const characters = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
        let result = ''

        for (let i = 0; i < 20; i++) {
            const randomIndex = Math.floor(Math.random() * characters.length)
            result += characters.charAt(randomIndex)
        }

        return result
    }

    function generateRandomKey() {
        const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789/+'
        let result = ''

        for (let i = 0; i < 43; i++) {
            const randomIndex = Math.floor(Math.random() * characters.length)
            result += characters.charAt(randomIndex)
        }

        result += '='
        return result
    }
}
