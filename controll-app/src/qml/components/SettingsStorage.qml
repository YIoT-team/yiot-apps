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
    }

    // KeyChain Writer
    WritePasswordJob {
        id: storeJobObject
        service: "yiot"
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
        service: "yiot"

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
