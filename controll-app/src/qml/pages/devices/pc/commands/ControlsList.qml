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
    id: rpiSettingsPage

    property var controller

    background: Rectangle {
        color: "transparent"
    }

    ListView {
        id: list
        anchors.fill: parent

        spacing: 1

        model: ListModel {
            ListElement {
                name: qsTr("Rename device")
                image: "create-user"
                property var action: function(ctx) {
                    showRenameDevicePage(ctx)
                }
            }

            ListElement {
                name: qsTr("Create User")
                image: "create-user"
                property var action: function(ctx) {
                    showCreateUserPage()
                }
            }
            ListElement {
                name: qsTr("Static IP")
                image: "static-ip"
                property var action: function(ctx) {
                    showStaticipPage()
                }
            }
            //ListElement {
            //    name: qsTr("Access Point")
            //    image: "access-point"
            //    property var action: function(ctx) {
            //        showAccessPointPage()
            //    }
            //}
            ListElement {
                name: qsTr("Enable SSH")
                image: "vpn-router"
                property var action: function(ctx) {
                    showSSHPage()
                }
            }

            ListElement {
                name: qsTr("Create WiFi VPN bridge")
                image: "vpn-router"
                property var action: function(ctx) {
                    showVPNrouterPage()
                }
            }
        }

        delegate: Rectangle {
            id: base
            width: parent.width
            height: 45
            color: "transparent"

            RowLayout {
                id: listDelegate
                anchors.fill: parent
                clip: true

                Image {
                    id: icon
                    source: "qrc:/qml/resources/icons/%1/%2.png".arg(Theme.state).arg(image)
                    Layout.maximumHeight: listDelegate.height * 0.7
                    Layout.maximumWidth: Layout.maximumHeight
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: 10
                }

                Text {
                    id: nameText
                    text: name
                    color: Theme.primaryTextColor
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: UiHelper.fixFontSz(14)

                    Layout.alignment: Qt.AlignLeft
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                RowLayout {
                    id: actionsBlock
                    Layout.rightMargin: 10

                    Image {
                        id: iconAction
                        source: "qrc:/qml/resources/icons/Arrow-Right.png"
                        Layout.maximumHeight: listDelegate.height * 0.7
                        Layout.maximumWidth: Layout.maximumHeight
                        fillMode: Image.PreserveAspectFit
                        Layout.alignment: Qt.AlignRight
                        Layout.rightMargin: 10
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    console.log(">>> ", rpiSettingsPage.controller)
                    action(rpiSettingsPage.controller)
                }

                onEntered: {
                    base.color = Theme.contrastBackgroundColor
                }

                onExited: {
                    base.color = "transparent"
                }
            }
        }
    }
}
