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
import "../../components/devices"
import "../../components/validators"

Popup {
    property string deviceMac: ""
    property alias name: deviceName.text
    property alias inform: informText.text
    property var actionOk: function(name) {  }
    property var actionClose: function(name) {  }
    property var ctx

    id: popup

    anchors.centerIn: parent
    width: parent.width * 0.9
    height: 200
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape

    background: Rectangle {
        border.color: Theme.primaryTextColor
        border.width: 1
        radius: 10
        color: Theme.mainBackgroundColor
    }

    ColumnLayout {
        anchors.fill: parent

        InfoText { id: deviceName }
        InfoText { id: informText; wrapMode: Text.WordWrap }

        RowLayout {
            Layout.fillWidth: true
            spacing: 20

            FormSecondaryButton {
                Layout.topMargin: 20
                Layout.bottomMargin: 10
                text: qsTr("Ok")
                onClicked: {
                    actionOk(ctx)
                    popup.close()
                }
            }

            FormSecondaryButton {
                Layout.topMargin: 20
                Layout.bottomMargin: 10
                text: qsTr("Cancel")
                onClicked: {
                    actionClose(ctx)
                    popup.close()
                }
            }
        }
    }

    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
    }

    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
    }
}
