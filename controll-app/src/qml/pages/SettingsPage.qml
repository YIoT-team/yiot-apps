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
        swipeSettingsShow(settingsSwipeView.wifiNetworksIdx)
    }

    function showWiFiPassword(ssid) {
        console.log("showWiFiPassword: ", ssid)
        wifiPassSettingsPage.prepare(ssid)
        swipeSettingsShow(settingsSwipeView.wifiPassIdx)
    }

    function showEventsSettings() {
        swipeSettingsShow(settingsSwipeView.eventsIdx)
    }

    function backInSettings() {
        swipeSettingsShow(settingsSwipeView.backPageIdx)
    }
}
