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

import "./settings"
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
        readonly property int createUserIdx: 1
        readonly property int staticipIdx: 2
        readonly property int accessPointIdx: 3
        readonly property int vpnRouterIdx: 4

        property int backPageIdx: listIdx

        id: settingsSwipeView
        anchors.fill: parent
        interactive: false
        currentIndex: listIdx

        ControlsList {
            id: settingsListPage
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

        VPNrouter {
            id: vpnRouterPage
        }
    }

    onVisibleChanged: {
        if (visible) {
            showRPiSettings()
        }
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

    function showStaticipPage() {
        swipeSettingsShow(settingsSwipeView.staticipIdx)
    }

    function showAccessPointPage() {
        swipeSettingsShow(settingsSwipeView.accessPointIdx)
    }

    function showVPNrouterPage() {
        swipeSettingsShow(settingsSwipeView.vpnRouterIdx)
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
