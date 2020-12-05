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

import "../../components"
import "../../components/devices"
import "../../components/RoT"

Page {
    id: eventsSettingsPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Root of trust")
        backAction: function() { showMenuSettings() }
    }

    RoTInfoPopup {
        id: rotInfo
    }

    Form {
        RoTList {}
    }

    ColumnLayout {
        anchors.topMargin: 1
        anchors.fill: parent
        RoTList {
            id: rotList
            Layout.fillHeight: true
            model: rotModel
        }

        FormPrimaryButton {
            Layout.bottomMargin: 10
            text: qsTr("Add new Root of trust")
            onClicked: {
                console.log("Add root of trust")
            }
        }
    }

    // Fill data and show device info.
    function showRoTInfo(model) {
        rotInfo.model = model
        rotInfo.open()
    }
}
