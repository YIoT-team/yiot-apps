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

import "./commands"
import "../../../theme"
import "../../../components"
import "../../../components/validators"

Page {
    property var controller
    property alias deviceName: header.title

    id: rpiPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        id: header
        title: qsTr("RPi Controll")
        backAction: function() { showDevices() }
    }

    SwipeView {
        readonly property int listIdx: 0
        readonly property int renameDeviceIdx: 1
        readonly property int createUserIdx: 2
        readonly property int staticipIdx: 3
        readonly property int accessPointIdx: 4
        readonly property int sshIdx: 5
        readonly property int vpnRouterIdx: 6

        property int backPageIdx: listIdx

        id: settingsSwipeView
        anchors.fill: parent
        interactive: false
        currentIndex: listIdx

        ControlsList {
            id: settingsListPage
        }

        RenameDevice {
            id: renameDevicePage
        }

        CreateUser {
            id: createUserPage
        }

        StaticIP {
            id: staticipPage
        }

        AccessPoint {
            id: accessPointPage
        }

        SSHEnabler {
            id: sshPage
        }

        VPNrouter {
            id: vpnRouterPage
        }
    }

    Connections {
        id: connections

        function onNameChanged() {
            deviceName = controller.name
        }
    }

    onVisibleChanged: {
        if (visible) {
            showRPiSettings()
        }
    }

    onControllerChanged: {
        connections.target = controller
        settingsListPage.controller = controller
    }

    function swipeSettingsShow(idx) {
        settingsSwipeView.currentIndex = idx
        if (idx != 0) {
            header.visible = false;
        } else {
            header.visible = true;
        }

        for (var i = 0; i < settingsSwipeView.count; ++i) {
            var item = settingsSwipeView.itemAt(i)
            item.visible = i == settingsSwipeView.currentIndex
        }
    }

     function showRPiSettings() {
        swipeSettingsShow(settingsSwipeView.listIdx)
     }

    function showCreateUserPage() {
        swipeSettingsShow(settingsSwipeView.createUserIdx)
    }

    function showRenameDevicePage(controller) {
            renameDevicePage.controller = controller
            swipeSettingsShow(settingsSwipeView.renameDeviceIdx)
        }

    function showStaticipPage() {
        swipeSettingsShow(settingsSwipeView.staticipIdx)
    }

    function showAccessPointPage() {
        swipeSettingsShow(settingsSwipeView.accessPointIdx)
    }

    function showVPNrouterPage() {
        swipeSettingsShow(settingsSwipeView.vpnRouterIdx)
    }

    function showSSHPage() {
        swipeSettingsShow(settingsSwipeView.sshIdx)
    }

    function backInRPiSettings() {
        swipeSettingsShow(settingsSwipeView.backPageIdx)
    }

    function errorPopupClick() {
    }

    function validateInputs() {
        if (userName.text == "") {
            showPopupError(qsTr("Set new User name"), errorPopupClick)
            return false
        }

        if (pass1.text == "") {
            showPopupError(qsTr("Set new User password"), errorPopupClick)
            return false
        }

        if (pass1.text != pass2.text) {
            showPopupError(qsTr("Passwords are not equal"), errorPopupClick)
            return false
        }

        if (staticIP.text == "") {
            showPopupError(qsTr("Static IP address isn't valid"), errorPopupClick)
            return false
        }

        return true
    }

}