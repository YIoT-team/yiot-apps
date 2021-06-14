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
    property var device: null

    id: p
    anchors.fill: parent

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Device initialisation")
        hideButtons: true
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
            }

            AnimatedImage {
                id: animatedImage

                Layout.fillHeight: true
                Layout.fillWidth: true

                fillMode: Image.PreserveAspectFit
                source: "qrc:/qml/resources/icons/%1/upload.gif".arg(Theme.state)
            }

            ScrollView {
                id: scrollProcessingText

                Layout.fillHeight: true
                Layout.fillWidth: true

                Layout.bottomMargin: p.state == "done" ? 450 : 20 // will need to be redone or removed altogether

//                TextArea {
//                    id: processingTextArea
//
//                    font.family: Theme.mainFont
//                    color: Theme.primaryTextColor
//                    readOnly: true
//
//                    text: //PCDevice.processingText()
//                }
            }

            FormSecondaryButton {
                id: actionButton
                Layout.bottomMargin: 10
                onClicked: {
                    startDeviceSetup(device)
                }
            }
        }
    }

    state: device == null ? "connect" : device.commandState
    states: [
        State {
            name: "connect"
            PropertyChanges { target: infoText; text: qsTr("Connecting") }
            PropertyChanges { target: infoText; color: Theme.brandColor }
            PropertyChanges { target: animatedImage; visible: true }
            PropertyChanges { target: actionButton; text: qsTr("Stop") }
        },
        State {
            name: "send"
            PropertyChanges { target: infoText; text: qsTr("Data send") }
            PropertyChanges { target: infoText; color: Theme.brandColor }
            PropertyChanges { target: animatedImage; visible: true }
            PropertyChanges { target: actionButton; text: qsTr("Stop") }
        },
        State {
            name: "receive"
            PropertyChanges { target: infoText; text: qsTr("Processing ...") }
            PropertyChanges { target: infoText; color: Theme.brandColor }
            PropertyChanges { target: animatedImage; visible: true }
            PropertyChanges { target: actionButton; text: qsTr("Stop") }
        },
        State {
            name: "disconnect"
            PropertyChanges { target: infoText; text: qsTr("Disconnecting") }
            PropertyChanges { target: infoText; color: Theme.brandColor }
            PropertyChanges { target: animatedImage; visible: true }
            PropertyChanges { target: actionButton; text: qsTr("Stop") }
        },
        State {
            name: "done"
            PropertyChanges { target: infoText; text: qsTr("DONE") }
            PropertyChanges { target: infoText; color: Theme.succesTextColor }
            PropertyChanges { target: animatedImage; visible: false }
            PropertyChanges { target: actionButton; text: qsTr("Ok") }
        },
        State {
            name: "error"
            PropertyChanges { target: infoText; text: qsTr("Error occured :(") }
            PropertyChanges { target: infoText; color: Theme.errorTextColor }
            PropertyChanges { target: animatedImage; visible: false }
            PropertyChanges { target: actionButton; text: qsTr("Ok") }
        }
    ]
}
