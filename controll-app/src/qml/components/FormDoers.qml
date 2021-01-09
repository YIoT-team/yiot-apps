import QtQuick 2.12
import QtQuick.Layouts 1.12

import "../theme"
import "../doers"

ColumnLayout {

    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.alignment: Qt.AlignHCenter
    Layout.leftMargin: 30
    Layout.rightMargin: Layout.leftMargin

    spacing: 10

    DoersTypeList {
        title: qsTr("Contributors")
        model: Contributors {}
    }

    DoersTypeList {
        title: qsTr("Sponsors")
        model: Sponsors {}
    }
}
