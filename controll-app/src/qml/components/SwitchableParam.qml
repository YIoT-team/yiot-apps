import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 1.15

import "../theme"

RowLayout {

    property alias infoText: info.text

    Layout.fillWidth: true
    Layout.topMargin: 10
    Layout.leftMargin: 10
    Layout.rightMargin: 10

    InfoText {
        id: info
        Layout.alignment: Qt.AlignLeft
        text: infoText
        font.pointSize: 12
    }

    Switch {
        id: stateSwitch

        Layout.alignment: Qt.AlignRight

//        checked: modelData.deviceController.state != "on"
//
//        onClicked: {
//            if (modelData.state !== "on") {
//                modelData.deviceController.setStateToHardware("on")
//            } else {
//                modelData.deviceController.setStateToHardware("off")
//            }
//        }

    }
}
