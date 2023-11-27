import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 1.5

import "../theme"

Label {
    id: formField
    Layout.preferredWidth: parent.width
    Layout.alignment: Qt.AlignHCenter
    color: Theme.primaryTextColor
    font.pointSize: 24
    horizontalAlignment: Text.AlignHCenter
    z: 100
    background: Rectangle {
        color: Theme.mainBackgroundColor

        Rectangle {
            width: parent.width
            height: 1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            color: Theme.brandColor
        }
    }
}
