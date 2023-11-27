import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 1.15

import "../theme"

Text {
    Layout.fillWidth: true
    Layout.alignment: Qt.AlignHCenter

    color: Theme.labelColor
    font.letterSpacing: 0.4
    font.bold: false
    font.family: Theme.mainFont
    font.pointSize: 12
    textFormat: Text.RichText
    linkColor: Theme.buttonPrimaryColor
    onLinkActivated: Qt.openUrlExternally(link)
}
