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

import "qrc:/qml/theme"
import "qrc:/qml/components"

Page {
    id: p
    property var device: ({ logo: "", name: "", version: "", description: "" })
    property var backAction: function() {  }

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("YIoT device")
        showBackButton: true
        showMenuButton: false
        showSettingsButton: false
        backAction: p.backAction
    }

    Form {
        stretched: true

        Image {
            Layout.maximumWidth: 100
            Layout.maximumHeight: 100

            fillMode: Image.PreserveAspectFit
            source: device.logo
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            text: device.name
            font.weight: Font.Bold
            font.capitalization: Font.Capitalize
            font.family: Theme.mainFontBold
            font.pointSize: 24
            color: Theme.brandColor
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            width: 180
            text: device.version
            lineHeight: 1
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter

            font.pointSize: 13
            font.letterSpacing: 0.3
            color: Theme.brandColor
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 60

            Layout.leftMargin: 20
            Layout.rightMargin: 20
            verticalAlignment: Text.AlignVCenter
            textFormat: TextEdit.MarkdownText
            text: device.description
            color: Theme.primaryTextColor
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        //-----------------------------------------------------------------------

        Text {
            id: label
            text: "Plugins:"
            color: Theme.brandColor
            font.weight: Font.Bold
            font.pointSize: 14
            verticalAlignment: Text.AlignBottom

            Layout.leftMargin: 20
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 65
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 20
            spacing: 15
            clip: true

            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: extensionPlugins
                delegate: Rectangle {
                    id: base
                    width: parent.width
                    height: 55
                    color: "transparent"

                    RowLayout {
                        id: listDelegate
                        anchors.fill: parent
                        clip: true

                        Image {
                            id: icon
                            source: info.logo
                            Layout.maximumHeight: listDelegate.height * 0.7
                            Layout.maximumWidth: Layout.maximumHeight
                            fillMode: Image.PreserveAspectFit
                            Layout.alignment: Qt.AlignLeft
                            Layout.leftMargin: 10
                        }

                        Text {
                            text: info.name
                            color: Theme.primaryTextColor
                            Layout.fillWidth: true
                        }

                        Text {
                            text: info.version
                            color: Theme.primaryTextColor
                            horizontalAlignment: Text.AlignRight
                            Layout.rightMargin: 15
                            Layout.fillWidth: true
                        }
                    }

                    MouseArea {
                        enabled: true
                        anchors.fill: parent
                        hoverEnabled: true
                        anchors.rightMargin: 0
                        onClicked: {
                            showPluginPage(info, function() {
                                showSettings()
                                showExtDevicesList()
                            })
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

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 25
            }
        }
        //-----------------------------------------------------------------------

        FormSecondaryButton {
            visible: false
            Layout.topMargin: 20
            Layout.bottomMargin: 30
            text: qsTr("Install")
            onClicked: {

            }
        }
    }
}
