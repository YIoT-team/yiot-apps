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
    property var model

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("Root of trust")
        showBackButton: true
        showMenuButton: false
        showSettingsButton: false
        backAction: function() {
            showRoTSettings()
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

            GridLayout {
                id: grid
                Layout.topMargin: 10
                Layout.leftMargin: 30

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                columns: 1

                InfoText { text: qsTr("Name: ") + f("name") }
                InfoText { text: qsTr("ID: ") + f("id") }

                InfoText { text: qsTr("Default EC: ") + f("ecType") }

                InfoText { text: qsTr("TrustList version: ") + fTl("version") }
                InfoText { text: qsTr("TrustList keys count: ") + fTl("keysCount") }
            }
        }
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        FormPrimaryButton {
            Layout.bottomMargin: 10
            text: qsTr("Export")
            onClicked: {
                console.log("Export root of trust")
                showRoTExportPage(model)
            }
        }

        FormSecondaryButton {
            Layout.bottomMargin: 10
            text: qsTr("Import")
            onClicked: {
                console.log("Import root of trust")
                showRoTImportPage(model)
            }
        }
    }

    function f(n) {
        if (typeof model !== 'undefined') {
            return model[n]
        }
        return ""
    }

    function fTl(n) {
        if (typeof model !== 'undefined') {
            return model.trustList[n]
        }
        return ""
    }
}
