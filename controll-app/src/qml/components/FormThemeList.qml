import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 1.15

import "../theme"

Button {
    id: button

    Layout.fillWidth: true
    Layout.maximumWidth: Theme.formMaximumWidth
    Layout.alignment: Qt.AlignHCenter

    property alias name: button.text

    contentItem: Text {
        text: button.text
        font.family: Theme.mainFont
        font.pointSize: 15
        font.letterSpacing: 0
        color: Theme.buttonPrimaryColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        color: "transparent"
        radius: height / 2
    }
}


