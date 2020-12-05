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

Popup {
    property variant model: none

    id: popup

    anchors.centerIn: parent
    width: parent.width * 0.9
    height: parent.height * 0.9
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        border.color: Theme.primaryTextColor
        border.width: 1
        radius: 10
        color: Theme.mainBackgroundColor
    }

    GridLayout {
        id: grid
        anchors.fill: parent
        columns: 1

        InfoText { text: qsTr("Name: ") + model.name }
        InfoText { text: qsTr("ID: ") + model.id }

        InfoText { text: qsTr("Default EC: ") + model.ecType }

        InfoText { text: qsTr("Recovery main: ") + model.recovery1 }
        InfoText { text: qsTr("Recovery reserv: ") + model.recovery2 }

        InfoText { text: qsTr("Auth main: ") + model.auth1 }
        InfoText { text: qsTr("Auth reserv: ") + model.auth2 }

        InfoText { text: qsTr("Tl main: ") + model.tl1 }
        InfoText { text: qsTr("Tl reserv: ") + model.tl2 }

        InfoText { text: qsTr("TrustList version: ") + model.trustList.version }
        InfoText { text: qsTr("TrustList keys count: ") + model.trustList.keysCount }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            popup.close()
        }
    }

    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
    }

    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
    }
}


