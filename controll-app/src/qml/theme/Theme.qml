pragma Singleton

import QtQuick 2.12
import QtQuick.Window 2.12

Item {
    FontLoader {
        id: robotoRegular
        source: "../resources/fonts/Roboto-Medium.ttf"
    }

    FontLoader {
        id: robotoBold
        source: "../resources/fonts/Roboto-Bold.ttf"
    }

    // colors
    readonly property color mainBackgroundColor : "#0A0A0A"
    readonly property color contrastBackgroundColor: "#222222"

    readonly property color color : "#ffffff"
    readonly property color brandColor: "white"
    readonly property color primaryTextColor: "#ffffff"
    readonly property color secondaryTextColor: "#e5e5e5"
    readonly property color linkTextColor: "#6189fa"
    readonly property color errorTextColor: "#f5425d"
    readonly property color succesTextColor: "#3cde6a"
    readonly property color labelColor: "#6F828C"
    readonly property color inputBackgroundColor: "#37474F"

    readonly property color buttonPrimaryColor: "#ffffff"
    readonly property color buttonPrimaryTextColor: "#0A0A0A"
    readonly property color buttonInvertedColor: "#0A0A0A"
    readonly property color buttonInvertedTextColor: "#ffffff"

    readonly property color tabImages : "#ffffff"



    // sizes
    readonly property real formMaximumWidth: 260

    // icons
    readonly property url mainLogo: "../resources/icons/Logo-Big.png"

    readonly property url btImg: "qrc:/qml/resources/icons/dark/bt.png"
    readonly property url wifiImg: "qrc:/qml/resources/icons/dark/wifi.png"

    // texts
    readonly property string mainLogoText: app.organizationDisplayName
    readonly property string mainFont: robotoRegular.name
    readonly property string mainFontBold: robotoBold.name
}
