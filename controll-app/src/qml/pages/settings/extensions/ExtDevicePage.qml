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
    property var device: null
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
            //anchors.horizontalCenter: parent.horizontalCenter
            Layout.alignment: Qt.AlignHCenter
        }

        Text {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            verticalAlignment: Text.AlignVCenter
            textFormat: TextEdit.MarkdownText
            text: device.description
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

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
