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

import "../theme"
import "../components"

Page {

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Devices setup")
        showBackButton: false
        showMenuButton: true
        showSettingsButton: true
    }

    ListView {
        id: list
        anchors.fill: parent
        anchors.topMargin: 1

        spacing: 1
        model: bleEnum

        delegate: Rectangle {
            id: base
            width: list.width
            height: 45
            color: "transparent"

            RowLayout {
                id: listDelegate
                anchors.fill: parent
                clip: true

                Image {
                    id: icon
                    source: Theme.btImg
                    Layout.maximumHeight: listDelegate.height * 0.7
                    Layout.maximumWidth: Layout.maximumHeight
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignLeft
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

                Text {
                    id: rssiText
                    text: rssi
                    color: Theme.primaryTextColor
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: UiHelper.fixFontSz(14)

                    Layout.alignment: Qt.AlignLeft
                    Layout.fillHeight: true
                    Layout.rightMargin: 5
                }

                RowLayout {
                    id: actionsBlock
                    Layout.rightMargin: 10

                    ImageButton {
                        id: btnInfo
                        image: "Search"
                        z: 100

                        onClicked: {
                            console.log("Click: Info")
                        }
                    }

                    ImageButton {
                        id: btnProvision
                        image: "dark/wifi-dimmed"

                        onClicked: { startProvision(name) }
                    }

                    states: [
                            State { when: list.currentIndex == index;
                                PropertyChanges {   target: actionsBlock; Layout.maximumWidth: 80    }
                            },
                            State { when: list.currentIndex != index;
                                PropertyChanges {   target: actionsBlock; Layout.maximumWidth: 0    }
                            }]

                    transitions: Transition {
                        NumberAnimation { property: "Layout.maximumWidth"; duration: 100}
                    }
                }
            }

            MouseArea {
                enabled: true
                anchors.fill: parent
                hoverEnabled: true
                anchors.rightMargin: list.currentIndex == index ? actionsBlock.width : 0
                onClicked: {
                    list.currentIndex = index
                }

                onEntered: {
                    list.currentIndex = index
                    base.color = Theme.contrastBackgroundColor
                }

                onExited: {
                    base.color = "transparent"
                }
            }
        }

        Component.onCompleted: {
        }

    }

    function startProvision(name) {
        var cred = settings.getWiFiCredDefault()
        if (cred.ready) {
            showCredLoad()
            bleController.configureWiFi(name, cred.ssid, cred.pass)
        } else {
            showPopupError(qsTr("Need to set WiFi credentials"), showSettingsForWiFi)
        }
    }

    function terminateProvision() {
        showDevicesSetup()
    }
}
