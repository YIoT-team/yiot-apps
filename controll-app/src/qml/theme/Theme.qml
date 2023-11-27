pragma Singleton

import QtQuick 2.15
import QtQuick.Window 2.12
import Qt.labs.settings 1.0

Item {
    id: themePage


    FontLoader {
        id: robotoRegular
        source: "../resources/fonts/Roboto-Medium.ttf"
    }

    FontLoader {
        id: robotoBold
        source: "../resources/fonts/Roboto-Bold.ttf"
    }

    // Save and restore theme settings
    Settings {
        id: sett
        property int saveMode: 1
        function restore() {
            getTheme(saveMode)
        }
        Component.onCompleted: restore();
    }


    readonly property color linkTextColor: "#6189fa"

    // colors
    property color mainBackgroundColor : "#0A0A0A"
    property color contrastBackgroundColor: "#222222"

    property color color : "#ffffff"
    property color brandColor: "white"
    property color primaryTextColor: "#ffffff"
    property color secondaryTextColor: "#e5e5e5"
    property color errorTextColor: "#f5425d"
    property color succesTextColor: "#3cde6a"
    property color labelColor: "#6F828C"
    property color inputBackgroundColor: "#37474F"

    property color buttonPrimaryColor: "#ffffff"
    property color buttonPrimaryTextColor: "#0A0A0A"
    property color buttonInvertedColor: "#0A0A0A"
    property color buttonInvertedTextColor: "#ffffff"

    property color tabImages : "#ffffff"
    property color contrastColor : "white"
    property string state: "dark"


    // sizes
    readonly property real formMaximumWidth: 260

    // icons
    readonly property url mainLogo: "../resources/icons/Logo-Big.png"

    property url btImg: "qrc:/qml/resources/icons/dark/bt.png"
    property url wifiImg: "qrc:/qml/resources/icons/dark/wifi.png"

    // texts
    readonly property string mainLogoText: app.organizationDisplayName
    readonly property string mainFont: robotoRegular.name
    readonly property string mainFontBold: robotoBold.name

    function getTheme(mode){
        if(mode === 1 ) {
            mainBackgroundColor = "#0A0A0A"
            contrastBackgroundColor = "#222222"
            color = "#ffffff"
            brandColor = "white"
            primaryTextColor = "#ffffff"
            secondaryTextColor = "#e5e5e5"
            errorTextColor = "#f5425d"
            succesTextColor = "#3cde6a"
            labelColor = "#6F828C"
            inputBackgroundColor = "#37474F"
            buttonPrimaryColor = "#ffffff"
            buttonPrimaryTextColor = "#0A0A0A"
            buttonInvertedColor = "#0A0A0A"
            buttonInvertedTextColor = "#ffffff"
            btImg = "qrc:/qml/resources/icons/dark/bt.png"
            wifiImg = "qrc:/qml/resources/icons/dark/wifi.png"
            tabImages = "#ffffff"
            contrastColor = "black"
            state = "dark"
        }
        if(mode === 0 ) {
            mainBackgroundColor = "#F5F5F5"
            contrastBackgroundColor = "#DDDDDD"
            color = "#000000"
            brandColor = "black"
            primaryTextColor = "#000000"
            secondaryTextColor = "#1a1a1a"
            errorTextColor = "#f5425d"
            succesTextColor = "#3cde6a"
            labelColor = "#6F828C"
            inputBackgroundColor = "#37474F"
            buttonPrimaryColor = "#000000"
            buttonPrimaryTextColor = "#F5F5F5"
            buttonInvertedColor = "#F5F5F5"
            buttonInvertedTextColor = "#000000"
            btImg = "qrc:/qml/resources/icons/light/bt.png"
            wifiImg = "qrc:/qml/resources/icons/light/wifi.png"
            tabImages = "#000000"
            contrastColor = "white"
            state = "light"

        }
        sett.saveMode = mode
        return 0;
    }
}
