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
    id: settingsPage

    property int darkMode: 1

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Global Settings")
        showBackButton: false
        showMenuButton: true
        showSettingsButton: true
    }

    ListView {
        id: list
        anchors.fill: parent

        spacing: 1

        model: ListModel {
            //ListElement {
            //    name: qsTr("Root of trust")
            //    image: "secure-enclave"
            //    property var action: function() {
            //        showRoTSettings()
            //    }
            //}

            //ListElement {
            //    name: qsTr("IoT Networks")
            //    image: "network"
            //    property var action: function() {
            //        showIoTNetworkSettings()
            //    }
            //}

            ListElement {
                name: qsTr("WiFi credentials")
                image: "creds"
                property var action: function() {
                    showWiFiSettings()
                }
            }

            //ListElement {
            //    name: qsTr("Event settings")
            //    image: "events"
            //    property var action: function() {
            //        showEventsSettings()
            //    }
            //}

            ListElement {
                name: qsTr("Theme")
                image: "themes"
                property var action: function() {
                    showThemeList()
                }
            }

            ListElement {
                name: qsTr("Plugins")
                image: "themes"
                property var action: function() {
                    showPluginsList(true)
                }
            }

            ListElement {
                name: qsTr("Test")
                image: "file:///Users/kutashenko/Downloads/docker.png"
                property var action: function() {
                    showDocker()
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
                    source: (image.indexOf("://") != -1) ? image : "qrc:/qml/resources/icons/%1/%2.png".arg(Theme.state).arg(image)
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
                    action()
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
