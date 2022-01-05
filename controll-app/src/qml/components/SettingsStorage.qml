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
import QtKeychain 1.0
import Qt.labs.settings 1.0

Item {
    signal loaded()

    property alias defaultWiFi: s.defaultWiFi

    property var wifiCache


    // Settings storage
    Settings {
        id: s
        property string defaultWiFi: ""

        property bool websockEnabled: false
        property string websockLink: ""

        property string servicesLogin: ""
    }

    // KeyChain Writer
    WritePasswordJob {
        id: storeJobObject
        service: "YIoT"
        autoDelete: false
        onFinished: {
            console.debug("Store password complete")
        }
    }

    // KeyChain Reader
    ReadPasswordJob {
        property var callback: function() {}

        id: readJobObject
        autoDelete: false
        service: "YIoT"

        Component.onCompleted: {
            readJobObject.finished.connect(function (returnedPassword) {
                var pass = returnedPassword.textData()

                // Add WiFi credentials to cache
                wifiCache[readJobObject.key] = pass

                // Invoke callback
                callback(readJobObject.key, pass)
            })
        }
    }

    function loadWiFiCred(ssid, callback) {
        readJobObject.key = ssid
        readJobObject.callback = callback
        readJobObject.start();
    }

    function getWiFiCred(ssid) {
        var r = false
        var p = ""

        if (ssid !== "") {
            p = wifiCache[ssid]
            if (typeof p === 'undefined') {
                p = ""
            }

            if (p === "") {
                loadWiFiCred(ssid, function (s, p) {
                    console.log("Loaded creds for: ", s)
                })
            } else {
                r = true
            }
        }

        return {
            ready: r,
            ssid: ssid,
            pass: p,
        };
    }

    function getWiFiCredDefault() {
        return getWiFiCred(s.defaultWiFi)
    }

    function setWiFiCredDefault(ssid, pass) {
        // Save password to KeyChain
        storeJobObject.key = ssid;
        storeJobObject.setTextData(pass);
        storeJobObject.start();

        // Add WiFi credentials to cache
        wifiCache[ssid] = pass

        // Set default WiFi Network in settings
        s.defaultWiFi = ssid
    }

    function setWebsocketState(state) {
        s.websockEnabled = state
    }

    function setWebsocketLink(link) {
        s.websockLink = link
    }

    function getWebsocketState() {
        return s.websockEnabled
    }

    function getWebsocketLink() {
        return s.websockLink
    }

    function setServicesLogin(login) {
        s.servicesLogin = login
    }

    function getServicesLogin() {
        return s.servicesLogin
    }

    Component.onCompleted: {
        // Prepare WiFi creds cache
        wifiCache = {}

        // Load default creds
        loadWiFiCred(s.defaultWiFi, function(s, p) {
            console.log("Loaded default WiFi creds: ", s)
            loaded()
        })
    }
}
