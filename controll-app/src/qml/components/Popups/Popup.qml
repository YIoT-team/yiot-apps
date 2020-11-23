import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    property alias popupView: popup
    property alias popupColor: popupBackground.color
    property alias popupColorText: message.color
    property bool popupModal: true
    property bool popupOnTop: false
    property var action: function() {  }

    Popup {
        id: popup

        property alias popMessage: message.text

        background: Rectangle {
            id: popupBackground
            implicitWidth: applicationWindow.width
            implicitHeight: 35
        }

        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
        }

        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
        }

        onAboutToHide: {
            popupClose.stop()
        }

        y: popupOnTop ? 0 : (applicationWindow.height - 35)
        modal: popupModal
        focus: popupModal

        closePolicy: Popup.CloseOnPressOutside

        Text {
            id: message
            anchors.centerIn: parent
            font.pointSize: UiHelper.fixFontSz(14)
        }
        onOpened: popupClose.start()

        MouseArea {
            anchors.fill: parent
            onClicked: {
                popup.close()
                action()
            }
        }
    }

    // Popup will be closed automatically in 2 seconds after its opened
    Timer {
        id: popupClose
        interval: 3000
        onTriggered: popup.close()
    }
}

