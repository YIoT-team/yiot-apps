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

Page {
    property alias pageTitle: h.title
//    property alias obj: connections.target

    id: p
    anchors.fill: parent
    state: "connect"

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

//    states: [
//        State {
//            name: "get-info"
//            PropertyChanges { target: processingForm; visible: true }
//            PropertyChanges { target: paramsForm; visible: false }
//        },
//        State {
//            name: "prepare-params"
//            PropertyChanges { target: processingForm; visible: false }
//            PropertyChanges { target: paramsForm; visible: true }
//        },
//        State {
//            name: "upload-setup"
//            PropertyChanges { target: processingForm; visible: true }
//            PropertyChanges { target: paramsForm; visible: false }
//        }
//    ]

    onVisibleChanged: {
        state = "get-info"
    }

//    Connections {
//        id: connections
//        target: deviceSetup
//
//        function onFireInitializationReady() {
//            state = "prepare-params"
//        }
//
//        function onFireError() {
//            state = "error"
//        }
//    }
}
