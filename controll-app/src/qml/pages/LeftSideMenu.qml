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

import QtQuick 2.5
import QtQuick.Layouts 1.5
import QtQuick.Controls 2.12

import "../theme"
import "../components"

Drawer {
    id: drawer
    dragMargin: 15
    width: 0.8 * parent.width
    height: parent.height
    clip: true

    background: Rectangle {
        color: Theme.contrastBackgroundColor
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 10
        ImageButton {
            id: btnClose
            image: "Menu"

            Layout.topMargin: 10
            Layout.alignment: Qt.AlignRight

            onClicked: drawer.close()
        }
    }

    Form {
        FormPrimaryButton {
            text: qsTr("Controlled devices")
            onClicked: {
                drawer.close()
                showDevices()
            }
        }

        FormPrimaryButton {
            text: qsTr("Devices setup")
            onClicked: {
                drawer.close()
                showDevicesSetup()
            }
        }

        FormPrimaryButton {
            text: qsTr("Services")
            onClicked: {
                drawer.close()
                showServices()
            }
        }

        FormPrimaryButton {
            text: qsTr("Settings")
            onClicked: {
                drawer.close()
                showSettings()
            }
        }

        FormPrimaryButton {
            text: qsTr("About")
            onClicked: {
                drawer.close()
                showAbout()
            }
        }
    }
}
