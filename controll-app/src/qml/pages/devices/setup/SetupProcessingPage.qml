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
import "../../../components/devices/setup"
import "../../settings"

Page {
    property alias pageTitle: h.title
    property alias obj: connections.target
    property bool wifiPassVisib: false

    id: p
    anchors.fill: parent
    state: "connect"

        function ssidCheck(aSsid) {
            if (aSsid != "") {
                wifiPassVisib = true
                state = "passwd"
                h.visible = false
                return (aSsid)
            } else {
                wifiPassVisib = false
                state = "prepare-params"
                h.visible = true
            }
        }

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        id: h
        title: qsTr("Credentials upload")
        hideButtons: true
    }

    ProcessingForm {
        id: processingForm
    }

    SetupParamsForm {
        id: paramsForm
    }

    WiFiPasswordPage {
        id: pass
        header: Header {
            id: header
            title: qsTr("WiFi password for current network.")
            backAction: function() { /*soon*/ }
        }
        visible: wifiPassVisib
        ssid: ssidCheck(wifiNetworksList.ssid)
    }

    Form {
        id: wifiList
        visible: false

        ColumnLayout {
            Layout.bottomMargin: 65
            Layout.fillWidth: true
            Layout.fillHeight: true

            WiFiNetworksList {
                id: wifiNetworksList
            }
        }
    }

    states: [
        State {
            name: "get-info"
            PropertyChanges { target: processingForm; visible: true }
            PropertyChanges { target: paramsForm; visible: false }
            PropertyChanges { target: wifiList; visible: false }
        },
        State {
            name: "prepare-params"
            PropertyChanges { target: processingForm; visible: false }
            PropertyChanges { target: paramsForm; visible: true }
            PropertyChanges { target: wifiList; visible: true }
        },
        State {
            name: "upload-setup"
            PropertyChanges { target: processingForm; visible: true }
            PropertyChanges { target: paramsForm; visible: false }
            PropertyChanges { target: wifiList; visible: false }
        },
        State {
            name: "passwd"
            PropertyChanges { target: processingForm; visible: false }
            PropertyChanges { target: paramsForm; visible: false }
            PropertyChanges { target: wifiList; visible: false }
        }
    ]

    onVisibleChanged: {
        state = "get-info"
    }

    Connections {
        id: connections
        target: deviceSetup

        function onFireInitializationReady() {
            state = "prepare-params"
        }

        function onFireUploadStarted() {
            state = "upload-setup"
            processingForm.configure()
        }

        Component.onCompleted: function() {
            ssid = wifiNetworksList.ssid
        }

    }
}
