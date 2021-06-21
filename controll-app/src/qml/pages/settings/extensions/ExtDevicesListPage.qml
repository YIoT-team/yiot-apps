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
import "qrc:/qml/components/Plugins"
import "qrc:/qml/theme"

Page {
    id: extensionDevicesListPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Device types and plugins")
        backAction: function() { showMenuSettings() }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 15

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: extensionDevices

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
                        Layout.fillWidth: true
                    }

                    Text {
                        text: info.version
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
                        showExtDevice(info, function() {
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
        }
    }
}
