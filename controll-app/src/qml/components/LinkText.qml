import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 1.15

import "../theme"

Text {
    property string link: ""
    verticalAlignment: Text.AlignVCenter
    font.pointSize: 14
    font.underline: ma.containsMouse
    color: ma.containsMouse ? Theme.linkTextColor : Theme.primaryTextColor
    Layout.alignment: Qt.AlignLeft

    MouseArea {
        id: ma
        enabled: link !== ""
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            Qt.openUrlExternally(link)
        }
    }
}
