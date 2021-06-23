import QtQuick 2.12
import QtQuick.Layouts 1.12

import "../theme"
import "../doers"

ColumnLayout {
    property alias title: t.text
    property alias model: l.model

    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.alignment: Qt.AlignHCenter
    Layout.leftMargin: 30
    Layout.rightMargin: Layout.leftMargin

    spacing: 10

    Title {
        id: t
        text: qsTr("Contributors")
        Layout.fillWidth: true
    }
    ListView {
        id: l
        Layout.fillWidth: true
        Layout.fillHeight: true

        delegate: Rectangle {
            width: parent.width
            height: 30
            color: "transparent"

            RowLayout {
                anchors.fill: parent
                clip: true

                Text {
                    id: nameText
                    text: name
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 14
                    font.underline: ma.containsMouse
                    color: ma.containsMouse ? Theme.linkTextColor : Theme.primaryTextColor

                    Layout.alignment: Qt.AlignLeft
                    Layout.fillHeight: true
                    Layout.fillWidth: true

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

            }

        }
    }

}
