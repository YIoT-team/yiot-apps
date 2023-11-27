import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../theme"

Column {

    Layout.fillWidth: true
    Layout.maximumWidth: Theme.formMaximumWidth
    Layout.alignment: Qt.AlignHCenter
    Layout.topMargin: 20

    spacing: 10

    Image {
        id: mainLogo
        width: 96
        height: 96

        fillMode: Image.PreserveAspectFit
        source: Theme.mainLogo
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: mainLogoText
        text: Theme.mainLogoText
        font.weight: Font.Bold
        font.capitalization: Font.Capitalize
        font.family: Theme.mainFontBold
        font.pointSize: 48
        color: Theme.brandColor
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: mainLogoDescription
        width: 180
        text: app.applicationVersion
        lineHeight: 1
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter

        font.pointSize: 13
        font.letterSpacing: 0.3
        color: Theme.brandColor
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
