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
import "qrc:/qml/components/validators"

Page {
    id: eventsSettingsPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("IoT Networks")
        backAction: function() { showMenuSettings() }
    }

    ColumnLayout {
        width: parent.width
        spacing: 20

        InputTextField {
            id: subnetIp
            Layout.topMargin: 20
            label: qsTr("Subnet address")
            placeholderText: qsTr("Enter subnet IP address")
            validator: ValidatorIPv4 {}
            inputMethodHints: Qt.ImhDigitsOnly
        }

        FormSecondaryButton {
            Layout.topMargin: 20
            Layout.bottomMargin: 10
            text: qsTr("Apply")
            onClicked: {
                app.setSubnet(subnetIp.text)
                settings.setSubnet(subnetIp.text)
                showDevices()
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Component.onCompleted: {
        subnetIp.text = settings.getSubnet()
        app.setSubnet(subnetIp.text)
    }
}
