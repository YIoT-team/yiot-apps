//  ────────────────────────────────────────────────────────────
//                     ╔╗  ╔╗ ╔══╗      ╔════╗
//                     ║╚╗╔╝║ ╚╣╠╝      ║╔╗╔╗║
//                     ╚╗╚╝╔╝  ║║  ╔══╗ ╚╝║║╚╝
//                      ╚╗╔╝   ║║  ║╔╗║   ║║
//                       ║║   ╔╣╠╗ ║╚╝║   ║║
//                       ╚╝   ╚══╝ ╚══╝   ╚╝
//    ╔╗╔═╗                    ╔╗                     ╔╗
//    ║║║╔╝                   ╔╝╚╗                    ║║
//    ║╚╝╝  ╔══╗ ╔══╗ ╔══╗  ╔╗╚╗╔╝  ╔══╗ ╔╗ ╔╗╔╗ ╔══╗ ║║  ╔══╗
//    ║╔╗║  ║║═╣ ║║═╣ ║╔╗║  ╠╣ ║║   ║ ═╣ ╠╣ ║╚╝║ ║╔╗║ ║║  ║║═╣
//    ║║║╚╗ ║║═╣ ║║═╣ ║╚╝║  ║║ ║╚╗  ╠═ ║ ║║ ║║║║ ║╚╝║ ║╚╗ ║║═╣
//    ╚╝╚═╝ ╚══╝ ╚══╝ ║╔═╝  ╚╝ ╚═╝  ╚══╝ ╚╝ ╚╩╩╝ ║╔═╝ ╚═╝ ╚══╝
//                    ║║                         ║║
//                    ╚╝                         ╚╝
//
//    Lead Maintainer: Roman Kutashenko <kutashenko@gmail.com>
//  ────────────────────────────────────────────────────────────

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../../components"
import "../../theme"

Page {
    id: themeSelectPage

    background: Rectangle {
        color: "transparent"

        property int themeState: 1
    }

    header: Header {
        id: header
        title: qsTr("Select theme")
        backAction: function() { showMenuSettings() }
    }

    ListView {
        id: list
        anchors.fill: parent

        spacing: 1

        model: ListModel {
            ListElement {
                name: qsTr("Dark")
                image: "moon"
                index: "dark"
                property var action: function() {
                    Theme.getTheme(1)
                }
            }
            ListElement {
                name: qsTr("Light")
                image: "sun"
                index: "light"
                property var action: function() {
                    Theme.getTheme(0)
                }
            }
        }

        delegate: Rectangle {
            id: base
            width: parent.width
            height: 45
            color: (Theme.state === index) ? Theme.contrastBackgroundColor : "transparent"

            RowLayout {
                id: listDelegate
                anchors.fill: parent
                clip: true

                Image {
                    id: icon
                    source: "qrc:/qml/resources/icons/%1/%2.png".arg(Theme.state).arg(image)
                    Layout.maximumHeight: listDelegate.height * 0.7
                    Layout.maximumWidth: Layout.maximumHeight
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: 10
                }

                Text {
                    id: nameText
                    text: name
                    color: Theme.primaryTextColor
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 14

                    Layout.alignment: Qt.AlignLeft
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }

            onVisibleChanged: { color = (Theme.state === index) ? Theme.contrastBackgroundColor : "transparent" }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    action()
                    showMenuSettings()
                }

                onEntered: {
                    base.color = Theme.contrastBackgroundColor
                }

                onExited: {
                    base.color = (Theme.state === index) ? Theme.contrastBackgroundColor : "transparent"
                }
            }
        }
    }
}
