import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../theme"

TabButton {
    id: btn
    property int idx: 0

    contentItem: Text {
        text: btn.text
        font.family: Theme.mainFont
        font.pointSize: UiHelper.fixFontSz(15)
        color: Theme.buttonPrimaryColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        height: 2
        width: 70
        y: parent.height - height
        anchors.horizontalCenter: parent.horizontalCenter
        color: tabBarPlugins.currentIndex == idx ? Theme.buttonPrimaryColor : Theme.buttonInvertedColor
    }
    onClicked: swipeShowPlugins(idx)
}
