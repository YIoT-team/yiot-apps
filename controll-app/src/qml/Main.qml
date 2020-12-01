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
import "./pages/devices"
import "./pages/settings"
import "./components"
import "./components/devices"
import "./components/Popups"
import "./theme"

ApplicationWindow {

    id: applicationWindow
    visible: true
    width: 400
    height: 700
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

    // TODO: Remove after complete adding of `CmdProcessingPage`
    // Page with Credentials upload information
    CredLoadPage { id: credLoad }

    // Page shows command processing process
    CmdProcessingPage { id: cmdProcessingPage }

    // Devices
    DeviceViewSelector {
        id: devicesSwipeView
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
        settings.loaded.connect(function() {
            app.updateDevices()
        })
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
                PropertyChanges { target: cmdProcessingPage; visible: false }
                PropertyChanges { target: devicesSwipeView; visible: false }
                PropertyChanges { target: swipeView; visible: false }
                PropertyChanges { target: tabBar; visible: false }
                PropertyChanges { target: leftSideMenu; enabled: false }
            },
            State {
                name: "main"
                PropertyChanges { target: aboutPage; visible: false }
                PropertyChanges { target: credLoad; visible: false }
                PropertyChanges { target: cmdProcessingPage; visible: false }
                PropertyChanges { target: devicesSwipeView; visible: false }
                PropertyChanges { target: swipeView; visible: true }
                PropertyChanges { target: tabBar; visible: true }
                PropertyChanges { target: leftSideMenu; enabled: true }
            },
            State {
                name: "credLoad"
                PropertyChanges { target: aboutPage; visible: false }
                PropertyChanges { target: credLoad; visible: true }
                PropertyChanges { target: cmdProcessingPage; visible: false }
                PropertyChanges { target: devicesSwipeView; visible: false }
                PropertyChanges { target: swipeView; visible: false }
                PropertyChanges { target: tabBar; visible: false }
                PropertyChanges { target: leftSideMenu; enabled: false }
            },
            State {
                name: "cmdProcessing"
                PropertyChanges { target: aboutPage; visible: false }
                PropertyChanges { target: credLoad; visible: false }
                PropertyChanges { target: cmdProcessingPage; visible: true }
                PropertyChanges { target: devicesSwipeView; visible: false }
                PropertyChanges { target: swipeView; visible: false }
                PropertyChanges { target: tabBar; visible: false }
                PropertyChanges { target: leftSideMenu; enabled: false }
            },
            State {
                name: "deviceControl"
                PropertyChanges { target: aboutPage; visible: false }
                PropertyChanges { target: credLoad; visible: false }
                PropertyChanges { target: cmdProcessingPage; visible: false }
                PropertyChanges { target: devicesSwipeView; visible: true }
                PropertyChanges { target: swipeView; visible: false }
                PropertyChanges { target: tabBar; visible: false }
                PropertyChanges { target: leftSideMenu; enabled: false }
            }
        ]
    }

    function swipeShow(idx) {
        w.state = "main"
        swipeView.currentIndex = idx
        for (var i = 0; i < swipeView.count; ++i) {
            var item = swipeView.itemAt(i)
            item.visible = i == swipeView.currentIndex
        }
    }

    // ------------------------------------------------------------------------
    //      Top level Views
    // ------------------------------------------------------------------------
    function showLeftMenu() {
        leftSideMenu.open()
    }

    function showAbout() {
        w.state = "about"
    }

    function showCredLoad() {
        w.state = "credLoad"
    }

    function showCmdProcessing(device) {
        cmdProcessingPage.device = device
        w.state = "cmdProcessing"
    }

    function showMain() {
        w.state = "main"
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

    // ------------------------------------------------------------------------
    //      Settings elements
    // ------------------------------------------------------------------------
    function showSettingsElement(idx) {
        swipeShow(swipeView.settingsPageIdx)
        settingsPage.swipeSettingsShow(idx)
    }

    function showSettingsForWiFi() {
        showSettingsElement(settingsPage.wifiNetworksIdx)
    }

    // ------------------------------------------------------------------------
    //      Popup messages
    // ------------------------------------------------------------------------
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

    // ------------------------------------------------------------------------
    //      Show Per device Views
    // ------------------------------------------------------------------------
    function showLampMono(deviceName, deviceController) {
        w.state = "deviceControl"
        devicesSwipeView.show(devicesSwipeView.lampMonoPageIdx,
                              deviceName,
                              deviceController)
    }

    function showPC(deviceName, deviceController) {
        w.state = "deviceControl"
        devicesSwipeView.show(devicesSwipeView.pcPageIdx,
                              deviceName,
                              deviceController)
    }
}
