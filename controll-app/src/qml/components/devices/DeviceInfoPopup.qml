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

Popup {
    property string mName: ""
    property string mAddr: ""
    property string mID: ""
    property string mManufacturer: ""
    property string mType: ""
    property string mRoles: ""
    property string mFwVer: ""
    property string mTlVer: ""
    property string mSent: ""
    property string mReceived: ""

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
        columns: 2

        InfoText { text: qsTr("Name :"); horizontalAlignment: Text.AlignRight }
        InfoText { text: mName }

        InfoText { text: qsTr("Addr :"); horizontalAlignment: Text.AlignRight }
        InfoText { text: mAddr }

        InfoText { text: qsTr("ID :"); horizontalAlignment: Text.AlignRight }
        InfoText { text: mID }

        InfoText { text: qsTr("Manufacturer :"); horizontalAlignment: Text.AlignRight }
        InfoText { text: mManufacturer }

//        InfoText { text: qsTr("Type :"); horizontalAlignment: Text.AlignRight }
//        InfoText { text: mType }
//
//        InfoText { text: qsTr("Roles :"); horizontalAlignment: Text.AlignRight }
//        InfoText { text: mRoles }

        InfoText { text: qsTr("Firmware version :"); horizontalAlignment: Text.AlignRight }
        InfoText { text: mFwVer }

        InfoText { text: qsTr("TrustList version :"); horizontalAlignment: Text.AlignRight }
        InfoText { text: mTlVer }

        InfoText { text: qsTr("Sent packets :"); horizontalAlignment: Text.AlignRight }
        InfoText { text: mSent }

        InfoText { text: qsTr("Received packets :"); horizontalAlignment: Text.AlignRight }
        InfoText { text: mReceived }
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


