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

import QtQuick 2.5
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.5

import "./pages"
import "./pages/devices/lamp/mono"
import "./pages/settings"
import "./components"
import "./components/Popups"
import "./theme"

ApplicationWindow {

    id: applicationWindow
    visible: true
    width: 400
    height: 600
    title: app.applicationDisplayName

    background: Rectangle {
        color: Theme.mainBackgroundColor
    }

    // Left-side menu
    LeftSideMenu { id: leftSideMenu }

    // Information popup
    Popup { id: inform }

    // About application page
    AboutPage { id: aboutPage }

    // Page with Credentials upload information
    CredLoadPage { id: credLoad }

    // Devices
    SwipeView {
        readonly property int lampMonoPageIdx: 0

        id: devicesSwipeView
        anchors.fill: parent
        interactive: false
        currentIndex: lampMonoPageIdx

        LampMonoControl { id: lampMonoPage }
    }

    // Main pages
    SwipeView {
        readonly property int devicePageIdx: 0
        readonly property int setupDevicePageIdx: 1
        readonly property int sharePageIdx: 2
        readonly property int settingsPageIdx: 3

        property int backPageIdx: devicePageIdx

        id: swipeView
        anchors.fill: parent
        interactive: false
        currentIndex: devicePageIdx

        DevicesPage { id: devicesPage }
        DevicesSetupPage { id: devicesSetupPage }
        SharePage { id: sharePage }
        SettingsPage { id: settingsPage }
    }

    // Manual switcher of main pages
    footer: TabBar {
        id: tabBar
        z: 5
        currentIndex: swipeView.currentIndex

        background: Rectangle {
            color: Theme.mainBackgroundColor
        }

        MainTabButton { idx: 0; image: "control-devices" }
        MainTabButton { idx: 1; image: "setup-devices" }
        MainTabButton { idx: 2; image: "share-access" }
        MainTabButton { idx: 3; image: "settings" }
    }

    // Settings container
    SettingsStorage { id: settings }

    Component.onCompleted: {
        showDevicesSetup()
    }

    StateGroup {
        id: w
        state: "main"

        states: [
            State {
                name: "about"
                PropertyChanges { target: aboutPage; visible: true }
                PropertyChanges { target: credLoad; visible: false }
                PropertyChanges { target: devicesSwipeView; visible: false }
                PropertyChanges { target: swipeView; visible: false }
                PropertyChanges { target: tabBar; visible: false }
                PropertyChanges { target: leftSideMenu; enabled: false }
            },
            State {
                name: "main"
                PropertyChanges { target: aboutPage; visible: false }
                PropertyChanges { target: credLoad; visible: false }
                PropertyChanges { target: devicesSwipeView; visible: false }
                PropertyChanges { target: swipeView; visible: true }
                PropertyChanges { target: tabBar; visible: true }
                PropertyChanges { target: leftSideMenu; enabled: true }
            },
            State {
                name: "credLoad"
                PropertyChanges { target: aboutPage; visible: false }
                PropertyChanges { target: credLoad; visible: true }
                PropertyChanges { target: devicesSwipeView; visible: false }
                PropertyChanges { target: swipeView; visible: false }
                PropertyChanges { target: tabBar; visible: false }
                PropertyChanges { target: leftSideMenu; enabled: false }
            },
            State {
                name: "deviceControl"
                PropertyChanges { target: aboutPage; visible: false }
                PropertyChanges { target: credLoad; visible: false }
                PropertyChanges { target: devicesSwipeView; visible: true }
                PropertyChanges { target: swipeView; visible: false }
                PropertyChanges { target: tabBar; visible: false }
                PropertyChanges { target: leftSideMenu; enabled: false }
            }
        ]
    }

    function showLeftMenu() {
        leftSideMenu.open()
    }

    function showAbout() {
        w.state = "about"
    }

    function showCredLoad() {
        w.state = "credLoad"
    }

    function showLampMono() {
        w.state = "deviceControl"
    }

    function showPC() {
        w.state = "deviceControl"
    }

    function showMain() {
        w.state = "main"
    }

    function swipeShow(idx) {
        w.state = "main"
        swipeView.currentIndex = idx
        for (var i = 0; i < swipeView.count; ++i) {
            var item = swipeView.itemAt(i)
            item.visible = i == swipeView.currentIndex
        }
    }

    function showDevices() {
        swipeShow(swipeView.devicePageIdx)
    }

    function showDevicesSetup() {
        swipeShow(swipeView.setupDevicePageIdx)
    }

    function showShared() {
        swipeShow(swipeView.sharePageIdx)
    }

    function showSettings() {
        swipeShow(swipeView.settingsPageIdx)
    }

    function showSettingsElement(idx) {
        swipeShow(swipeView.settingsPageIdx)
        settingsPage.swipeSettingsShow(idx)
    }

    function showSettingsForWiFi() {
        showSettingsElement(settingsPage.wifiNetworksIdx)
    }

    // Show Popup message
    function showPopup(message, color, textColor, isOnTop, isModal, action) {
        inform.popupColor = color
        inform.popupColorText = textColor
        inform.popupView.popMessage = message
        inform.popupOnTop = isOnTop
        inform.popupModal = isModal
        inform.action = action
        inform.popupView.open()
    }

    function showPopupError(message, action) {
        showPopup(message, Theme.buttonPrimaryColor, Theme.buttonPrimaryTextColor, true, true, action)
    }

    function showPopupInform(message) {
        // TODO: Add
    }
}
