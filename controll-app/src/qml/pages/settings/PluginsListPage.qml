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
import "../../components/Plugins"
import "../../theme"

Page {
    id: eventsSettingsPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        title: qsTr("YIoT plugins")
        backAction: function() { showMenuSettings() }
    }

        ColumnLayout {
            anchors.fill: parent

            spacing: 15

            TabBar {
                id: tabBarPlugins
                Layout.fillWidth: true

                z: 5
                currentIndex: swipeViewPlugins.currentIndex

                background: Rectangle {
                    color: Theme.mainBackgroundColor
                }

                TextTabButton { idx: 0; text: qsTr("Available") }
                TextTabButton { idx: 1; text: qsTr("Installed") }
            }

            SwipeView {
                    readonly property int availableIdx: 0
                    readonly property int installedIdx: 1

                    property int backPageIdx: availableIdx

                    id: swipeViewPlugins
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    interactive: false
                    currentIndex: availableIdx

                    PluginsAvailable { id: pluginsAvailable }
                    PluginsInstalled { id: pluginsInstalled }
                }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

    Component.onCompleted: {
        swipeShowPlugins(availableIdx)
    }

        function swipeShowPlugins(idx) {
            swipeViewPlugins.currentIndex = idx
            for (var i = 0; i < swipeViewPlugins.count; ++i) {
                var item = swipeViewPlugins.itemAt(i)
                item.visible = i == swipeViewPlugins.currentIndex
            }
        }
}
