import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.5

import "../theme"

ColumnLayout {
    property alias label: formLabel.text
    property alias text: formField.text
    property alias placeholderText: formField.placeholderText
    property alias validator: formField.validator
    property alias echoMode: formField.echoMode
    property alias inputMethodHints: formField.inputMethodHints
    property alias maximumLength: formField.maximumLength

    signal ready()

    Layout.preferredHeight: 30
    Layout.maximumWidth: parent.width > 60 ? parent.width - 60 : parent.width
    Layout.alignment: Qt.AlignHCenter

    spacing: 0

    FormLabel {
        id: formLabel
    }

    TextField {
        id: formField
        Layout.preferredWidth: parent.width
        Layout.alignment: Qt.AlignHCenter
        color: Theme.primaryTextColor
        font.pointSize: 14
        leftPadding: 30
        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 30
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

        onEditingFinished: ready()
    }

}
