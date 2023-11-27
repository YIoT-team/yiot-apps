import QtQuick 2.15
import QtQuick.Controls.Basic 2.15

Rectangle {
    property string variant
    property alias label: label.text
    color: variant === "error" ? "red" : "gray"
    height: visible ? 25 : 0

    Label {
        id: label
        anchors.centerIn: parent
        color: "white"
        font.pointSize: 12
    }
}
