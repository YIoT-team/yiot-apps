import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../theme"

ToolBar {

    property alias title: titleLabel.text
    property bool showBackButton: true
    property bool showMenuButton: false
    property bool showSettingsButton: false
    property bool hideButtons: false
    property alias showSeporator: seporator.visible
    property alias backgroundColor: background.color
    property alias textClickEnable: ma.enabled
    property var backAction: function() { back() }
    property var settingsAction: function() { showAbout() }

    signal textClick()

    background: Rectangle {
        id: background
        implicitHeight: 60
        color: Theme.mainBackgroundColor

        Rectangle {
            id: seporator
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            anchors.rightMargin: 20

            height: 1
            color: Theme.contrastBackgroundColor
            anchors.bottom: parent.bottom
        }
    }

    RowLayout {
        anchors.fill: parent

        ImageButton {
            Layout.leftMargin: 12

            opacity: showBackButton ? 1 : 0
            enabled: showBackButton
            visible: !hideButtons && !showMenuButton

            id: backButton
            image: "Arrow-Left"

            // TODO: Componets sholdn't know about mainView
            // The logic must be as abastract as possible.
            onClicked: backAction()
        }

        ImageButton {
            id: btnProvision
            image: "Menu"

            opacity: showMenuButton ? 1 : 0
            enabled: showMenuButton
            visible: !hideButtons && !showBackButton

            Layout.leftMargin: 12

            onClicked: showLeftMenu()
        }

        Label {
            Layout.fillWidth: true

            id: titleLabel
            elide: Label.ElideRight
            textFormat: Text.MarkdownText

            font.underline: ma.containsMouse
            font.family: Theme.mainFont
            font.pointSize: 15
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            color: ma.containsMouse ? Theme.linkTextColor : Theme.primaryTextColor

            MouseArea {
                id: ma
                enabled: false
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    textClick()
                }
            }
        }

        ImageButton {
            Layout.rightMargin: 12

            id: menuButton
            image: "More"
             // TODO(fpohtmeh): try with visibility
            opacity: showSettingsButton ? 1 : 0
            enabled: showSettingsButton
            visible: !hideButtons
            onClicked: {
                settingsAction()
            }
        }
    }
}
