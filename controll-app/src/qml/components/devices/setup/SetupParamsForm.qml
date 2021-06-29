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

        Layout.leftMargin: 20
        Layout.rightMargin: 20

        spacing: 30

        GridLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            columns: 2
            columnSpacing: 10

            InfoText {
                text: qsTr("Device name:")
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: (form.width - 10) / 2
            }
            InfoText { text: deviceSetup.data.name; Layout.preferredWidth: (form.width - 10) / 2 }

            InfoText { text: qsTr("Manufacturer:"); horizontalAlignment: Text.AlignRight }
            InfoText { text: deviceSetup.data.manufacturer }

            InfoText { text: qsTr("Security is initialized:"); horizontalAlignment: Text.AlignRight }
            InfoText { text: deviceSetup.data.hasProvision ? qsTr("yes") : qsTr("no") }

            InfoText { text: qsTr("Owner is set:"); horizontalAlignment: Text.AlignRight }
            InfoText { text: deviceSetup.data.hasOwner ? qsTr("yes") : qsTr("no") }

            // InfoText { text: qsTr("Owned by me:"); horizontalAlignment: Text.AlignRight }
            // InfoText { text: deviceSetup.data.ownerIsYou ? qsTr("yes") : qsTr("no") }
        }

        InfoText {
            visible: !deviceSetup.data.hasProvision
            wrapMode: Text.WordWrap
            text: qsTr("This device will be provisioned using your Root Of Trust")
        }

        InfoText {
            visible: true
            wrapMode: Text.WordWrap
            text: qsTr("WiFi credentials will be passed as an encrypted data.")
        }

        InfoText {
            visible: deviceSetup.data.needCreds
            wrapMode: Text.WordWrap
            text: qsTr("This device requires WiFi credentials. Credentials will be passed as an encrypted data.")
        }

        Item {
            Layout.fillHeight: true
        }

        FormSecondaryButton {
            Layout.bottomMargin: 10
            text: deviceSetup.data.needCreds ? qsTr("Next") : qsTr("Setup Device")
            onClicked: {
                if (deviceSetup.data.needCreds) {
                    setWiFiPassLocation("deviceSetup")
                    setCredLoadState("wifi-setup")
                } else {
                    var cred = settings.getWiFiCredDefault()
                    deviceSetup.configure(
                        !deviceSetup.data.hasProvision,
                        !deviceSetup.data.hasOwner, "Me",
                        true, cred.ssid, cred.pass)

                    showCredLoad()
                }
            }
        }
    }
}
