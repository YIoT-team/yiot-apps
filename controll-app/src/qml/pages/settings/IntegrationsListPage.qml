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
import "../../components/Integrations"
import "../../theme"

Page {
    readonly property int installedIdx: 0
    readonly property int availableIdx: 1

    id: integrationsListPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: pluginsInstalled.controlPageOpen ? qsTr("Integrations") : qsTr("Websocket")
        backAction: function() {
            if (pluginsInstalled.controlPageOpen) {
                showMenuSettings()
            } else {
               pluginsInstalled.controlPageOpen = true
            }
        }
    }

    Loader {
           id: myLoader
           anchors.fill: parent
           visible: !pluginsInstalled.controlPageOpen
           source: "qrc:/integration/0/qml/ControlIntegrations.qml"
    }

    ColumnLayout {
        anchors.fill: parent

        visible: pluginsInstalled.controlPageOpen

        spacing: 15

        TabBar {
            id: tabBarPlugins
            Layout.fillWidth: true

            z: 5
            currentIndex: swipeViewPlugins.currentIndex

            background: Rectangle {
                color: Theme.mainBackgroundColor
            }

            TextTabButton { idx: 0; text: qsTr("Installed") }
            TextTabButton { idx: 1; text: qsTr("Available") }
        }

        SwipeView {
            property int backPageIdx: installedIdx

            id: swipeViewPlugins
            Layout.fillWidth: true
            Layout.fillHeight: true
            interactive: false
            currentIndex: installedIdx

            IntegrationsInstalled { id: pluginsInstalled }
            IntegrationsAvailable { id: pluginsAvailable }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Component.onCompleted: {
        swipeShowPlugins(installedIdx)
    }

    function swipeShowPlugins(idx) {
        swipeViewPlugins.currentIndex = idx
        for (var i = 0; i < swipeViewPlugins.count; ++i) {
            var item = swipeViewPlugins.itemAt(i)
            item.visible = i == swipeViewPlugins.currentIndex
        }
    }
}