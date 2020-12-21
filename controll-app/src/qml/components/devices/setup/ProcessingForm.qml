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
            font.pointSize: UiHelper.fixFontSz(24)
        }

        AnimatedImage {
            id: animatedImage

            Layout.fillHeight: true
            Layout.fillWidth: true

            fillMode: Image.PreserveAspectFit
            source: "qrc:/qml/resources/icons/dark/upload.gif"
        }

        FormPrimaryButton {
            id: actionButton
            Layout.bottomMargin: 10
            onClicked: {
                devicesSetupPage.terminateProvision()
            }
        }
    }

//    StateGroup {
//        id: stateGroup
//        state: "connect"
//    states: [
//        State {
//            name: "connect"
//            PropertyChanges { target: infoText; text: qsTr("Connecting") }
//            PropertyChanges { target: infoText; color: Theme.brandColor }
//            PropertyChanges { target: animatedImage; visible: true }
//            PropertyChanges { target: actionButton; text: qsTr("Stop") }
//        },
//        State {
//            name: "process"
//            PropertyChanges { target: infoText; color: Theme.brandColor }
//            PropertyChanges { target: animatedImage; visible: true }
//            PropertyChanges { target: actionButton; text: qsTr("Stop") }
//        },
//        State {
//            name: "error"
//            PropertyChanges { target: infoText; text: qsTr("Error occured :(") }
//            PropertyChanges { target: infoText; color: Theme.errorTextColor }
//            PropertyChanges { target: animatedImage; visible: false }
//            PropertyChanges { target: actionButton; text: qsTr("Ok") }
//        }
//    ]
//    }

    onVisibleChanged: {
        state = "connect"
    }

//    Connections {
//        id: connections
//        target: deviceSetup
//
//        function onFireStateInfo(text) {
//            state = "process"
//            infoText.text = text
//        }
//
//        function onFireError() {
//            state = "error"
//        }
//    }
}
