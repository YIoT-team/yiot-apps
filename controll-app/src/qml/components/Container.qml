import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 1.15

Item {
    anchors.fill: parent

    property alias content: body.contentItem
    property real maximumWidth: 280

    ColumnLayout {
        anchors.fill: parent

        Item {
            Layout.fillHeight: true
        }

        Control {
            id: body
            Layout.alignment: Qt.AlignHCenter
            Layout.maximumWidth: maximumWidth
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
