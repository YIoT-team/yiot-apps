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

import "qrc:/qml/pages/settings/extensions"
import "qrc:/qml/pages/settings"
import "qrc:/qml/pages/RoT"
import "qrc:/qml/components"

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
        readonly property int extDevicesIdx: 7
        //readonly property int integrationsIdx: 8
        readonly property int onePluginIdx: 8
        readonly property int oneServiceIdx: 9
        readonly property int oneDevicePageIdx: 10
        readonly property int rotControlPageIdx: 11
        readonly property int rotExportPageIdx: 12
        readonly property int rotImportPageIdx: 13

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

        ExtDevicesListPage {
            id: extDeviccesListPage
        }

//        ExtIntegrationsListPage {
//            id: extIntegrationsListPage
//        }

        ExtPluginPage {
            id: extPluginPage
        }

        ExtServicePage {
            id: extServicePage
        }

        ExtDevicePage {
            id: extDevicePage
        }

        RoTControlPage {
            id: rotControlPage
        }

        RoTExportPage {
            id: rotExportPage
        }

        RoTImportPage {
            id: rotImportPage
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
        wifiPassSettingsPage.prepareLocation("credentials") //TODO: Remove string parameter
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

    function showExtDevicesList() {
        swipeSettingsShow(settingsSwipeView.extDevicesIdx)
    }

    function showExtDevice(device, backAction) {
        extDevicePage.device = device
        extDevicePage.backAction = backAction
        swipeSettingsShow(settingsSwipeView.oneDevicePageIdx)
    }

//    function showIntegrationsList() {
//        swipeSettingsShow(settingsSwipeView.integrationsIdx)
//    }

    function showPluginPage(plugin, backAction) {
        extPluginPage.plugin = plugin
        extPluginPage.backAction = backAction
        swipeSettingsShow(settingsSwipeView.onePluginIdx)
    }

    function showServicePage(service, backAction) {
        swipeSettingsShow(settingsSwipeView.oneServiceIdx)
        extServicePage.show(service)
    }

    function backInSettings() {
        swipeSettingsShow(settingsSwipeView.backPageIdx)
    }
    function setPassPageLocation(location) {
        wifiPassSettingsPage.prepareLocation(location)
    }

    function showRoTControlPage(model) {
        rotControlPage.model = model
        swipeSettingsShow(settingsSwipeView.rotControlPageIdx)
    }

    function showRoTExportPage(model) {
        rotExportPage.model = model
        swipeSettingsShow(settingsSwipeView.rotExportPageIdx)
    }

    function showRoTImportPage(model) {
        swipeSettingsShow(settingsSwipeView.rotImportPageIdx)
        rotImportPage.start(model)
    }
}
