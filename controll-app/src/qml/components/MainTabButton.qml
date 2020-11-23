import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../theme"

TabButton {
    property int idx: 0
    property string image: "control-devices"

    property int iconSz: 50

    icon.color: Theme.tabImages
    icon.source: "../resources/icons/dark/%1.png".arg(image)
    icon.height: iconSz
    icon.width: iconSz
    background: Rectangle {
        height: 2
        width: iconSz
        anchors.horizontalCenter: parent.horizontalCenter
        color: tabBar.currentIndex == idx ? Theme.buttonPrimaryColor : Theme.buttonInvertedColor
    }
    onClicked: swipeShow(idx)
}