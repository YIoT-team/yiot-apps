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

import "../../../theme"
import "../../../components"

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
            font.pointSize: 24
        }

        AnimatedImage {
            id: animatedImage

            Layout.fillHeight: true
            Layout.fillWidth: true

            fillMode: Image.PreserveAspectFit
            source: "qrc:/qml/resources/icons/%1/upload.gif".arg(Theme.state)
        }

        FormSecondaryButton {
            id: actionButton
            Layout.bottomMargin: 10
            onClicked: {
                deviceSetup.stop()
            }
        }

        StateGroup {
            id: stateGroup
            state: "connect"
            states: [
                State {
                    name: "connect"
                    PropertyChanges { target: infoText; text: qsTr("Connecting") }
                    PropertyChanges { target: infoText; color: Theme.brandColor }
                    PropertyChanges { target: animatedImage; visible: true }
                    PropertyChanges { target: actionButton; text: qsTr("Stop") }
                },
                State {
                    name: "process"
                    PropertyChanges { target: infoText; color: Theme.brandColor }
                    PropertyChanges { target: animatedImage; visible: true }
                    PropertyChanges { target: actionButton; text: qsTr("Stop") }
                },
                State {
                    name: "error"
                    PropertyChanges { target: infoText; text: qsTr("Error occured :(") }
                    PropertyChanges { target: infoText; color: Theme.errorTextColor }
                    PropertyChanges { target: animatedImage; visible: false }
                    PropertyChanges { target: actionButton; text: qsTr("Ok") }
                },
                State {
                    name: "done"
                    PropertyChanges { target: infoText; text: qsTr("DONE") }
                    PropertyChanges { target: infoText; color: Theme.succesTextColor }
                    PropertyChanges { target: animatedImage; visible: false }
                    PropertyChanges { target: actionButton; text: qsTr("Ok") }
                }
            ]
        }

        Connections {
            id: connections
            target: deviceSetup

            function onFireStateInfo(text) {
                stateGroup.state = "process"
                infoText.text = text
            }

            function onFireError() {
                stateGroup.state = "error"
            }

            function onFireUploadDone() {
                stateGroup.state = "done"
            }
        }
    }

    onVisibleChanged: {
        stateGroup.state = "connect"
    }

    function configure() {
        stateGroup.state = "process"
        infoText.text = qsTr("Device configuration")
    }
}
