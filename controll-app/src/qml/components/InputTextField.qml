import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.5

import "../theme"

TextField {
    id: password
    Layout.preferredWidth: parent.width - 20
    Layout.alignment: Qt.AlignHCenter
    color: Theme.primaryTextColor
    font.pointSize: UiHelper.fixFontSz(14)
    leftPadding: 30
    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 50
        radius: implicitHeight / 2
        color: "transparent"

        Rectangle {
            width: parent.width - 10
            height: 1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            color: Theme.brandColor
        }
    }
}
