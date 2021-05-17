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

    readonly property string defaultHeaderText: qsTr("Device initialisation")
    readonly property string wifiSetupHeaderText: qsTr("Select WiFi network")

    id: p
    anchors.fill: parent
    state: "connect"

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        id: h
        title: qsTr(defaultHeaderText)
        backAction: function() { deviceSetup.stop(); showDevicesSetup() }
    }

    ProcessingForm {
        id: processingForm
    }

    SetupParamsForm {
        id: paramsForm
    }

    DeviceSetupWiFiListForm {
        id: wifiList
    }

    states: [
        State {
            name: "get-info"
            PropertyChanges { target: processingForm; visible: true }
            PropertyChanges { target: paramsForm; visible: false }
            PropertyChanges { target: wifiList; visible: false }
            PropertyChanges { target: h; title: defaultHeaderText }
        },
        State {
            name: "prepare-params"
            PropertyChanges { target: processingForm; visible: false }
            PropertyChanges { target: paramsForm; visible: true }
            PropertyChanges { target: wifiList; visible: false }
            PropertyChanges { target: h; title: defaultHeaderText }
        },
        State {
            name: "wifi-setup"
            PropertyChanges { target: processingForm; visible: false }
            PropertyChanges { target: paramsForm; visible: false }
            PropertyChanges { target: wifiList; visible: true }
            PropertyChanges { target: h; title: wifiSetupHeaderText }
        },
        State {
            name: "upload-setup"
            PropertyChanges { target: processingForm; visible: true }
            PropertyChanges { target: paramsForm; visible: false }
            PropertyChanges { target: wifiList; visible: false }
            PropertyChanges { target: h; title: defaultHeaderText }
        },
        State {
            name: "passwd"
            PropertyChanges { target: processingForm; visible: false }
            PropertyChanges { target: paramsForm; visible: false }
            PropertyChanges { target: wifiList; visible: false }
            PropertyChanges { target: h; title: defaultHeaderText }
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

    }
}
