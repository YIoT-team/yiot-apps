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
    property var controller
    property alias deviceName: header.title

    id: p

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        id: header
        title: qsTr("Lamp")
        backAction: function() { showDevices() }
        textClickEnable: true

        onTextClick: {
            devicesPage.showDeviceRenameDialog(deviceName, controller)
        }
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

            Label {
                id: infoText
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true
                Layout.fillWidth: true

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: Theme.mainFont
                font.pointSize: UiHelper.fixFontSz(24)
                color: Theme.brandColor
            }

            Image {
                property string img: "unknown"
                id: stateImage

                Layout.fillHeight: true
                Layout.fillWidth: true

                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/icons/dark/devices/lamp/mono/%1".arg(img)

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (controller.state !== "on") {
                            controller.setStateToHardware("on")
                        } else {
                            controller.setStateToHardware("off")
                        }
                    }
                }
            }

            FormPrimaryButton {
                id: actionButton
                Layout.bottomMargin: 10
                text: qsTr("Close")
                onClicked: {
                    showDevices()
                }
            }
        }
    }

    state: typeof controller === "undefined" ? "unknown" : controller.state
    states: [
        State {
            name: "unknown"
            PropertyChanges { target: infoText; text: qsTr("Unknown state") }
            PropertyChanges { target: stateImage; img: "unknown" }
        },
        State {
            name: "on"
            PropertyChanges { target: infoText; text: qsTr("On") }
            PropertyChanges { target: stateImage; img: "on" }
        },
        State {
            name: "off"
            PropertyChanges { target: infoText; text: qsTr("Off") }
            PropertyChanges { target: stateImage; img: "off" }
        }
    ]
}
