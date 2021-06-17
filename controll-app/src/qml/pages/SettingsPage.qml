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

import "./settings"
import "../components"

Page {
    readonly property int menuIdx: 0
    readonly property int wifiNetworksIdx: 1
    readonly property int wifiPassIdx: 2
    readonly property int eventsIdx: 3

    id: settingsPage

    background: Rectangle {
        color: "transparent"
    }

    SwipeView {
        readonly property int menuIdx: 0
        readonly property int wifiNetworksIdx: 1
        readonly property int wifiPassIdx: 2
        readonly property int eventsIdx: 3
        readonly property int rotIdx: 4
        readonly property int networksIdx: 5
        readonly property int themeIdx: 6
        readonly property int pluginsIdx: 7
        readonly property int integrationsIdx: 8

        property int backPageIdx: menuIdx

        id: settingsSwipeView
        anchors.fill: parent
        interactive: false
        currentIndex: menuIdx

        SettingsMenuPage {
            id: menuSettingsPage
        }

        WiFiSettingsPage {
            id: wifiSettingsPage
        }

        WiFiPasswordPage {
            id: wifiPassSettingsPage
        }

        EventsSettingsPage {
            id: eventsSettingsPage
        }

        RoTSettingsPage {
            id: rotSettingsPage
        }

        NetworksSettingsPage {
            id: networksSettingsPage
        }
        
        ThemeListPage {
            id: themeListPage
        }

        PluginsListPage {
            id: pluginsListPage
        }

        IntegrationsListPage {
            id: integrationsListPage
        }
    }

    onVisibleChanged: {
        if (visible) {
            showMenuSettings()
        }
    }

    function swipeSettingsShow(idx) {
        settingsSwipeView.currentIndex = idx
        for (var i = 0; i < settingsSwipeView.count; ++i) {
            var item = settingsSwipeView.itemAt(i)
            item.visible = i == settingsSwipeView.currentIndex
        }
    }

     function showMenuSettings() {
        swipeSettingsShow(settingsSwipeView.menuIdx)
     }

    function showWiFiSettings() {
        wifiPassSettingsPage.prepareLocation("credentials")
        swipeSettingsShow(settingsSwipeView.wifiNetworksIdx)
    }

    function showRoTSettings() {
        swipeSettingsShow(settingsSwipeView.rotIdx)
    }

    function showIoTNetworkSettings() {
        swipeSettingsShow(settingsSwipeView.networksIdx)
    }

    function showWiFiPassword(ssid) {
        console.log("showWiFiPassword: ", ssid)
        wifiPassSettingsPage.prepare(ssid)
    }

    function showEventsSettings() {
        swipeSettingsShow(settingsSwipeView.eventsIdx)
    }

    function showThemeList() {
        swipeSettingsShow(settingsSwipeView.themeIdx)
    }

    function showPluginsList(available) {
        if (available) {
            pluginsListPage.swipeShowPlugins(pluginsListPage.availableIdx)
        } else {
            pluginsListPage.swipeShowPlugins(pluginsListPage.installedIdx)
        }
        swipeSettingsShow(settingsSwipeView.pluginsIdx)
    }

    function showIntegrationsList() {
        swipeSettingsShow(settingsSwipeView.integrationsIdx)
    }

    function backInSettings() {
        swipeSettingsShow(settingsSwipeView.backPageIdx)
    }
    function setPassPageLocation(location) {
        wifiPassSettingsPage.prepareLocation(location)
    }
}
