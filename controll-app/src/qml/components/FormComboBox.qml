import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.5

import "../theme"

ColumnLayout {
    id: columnLayout
    property alias items: comboBox.items
    property alias text: comboBox.selectedItem

    Layout.fillWidth: true
    Layout.preferredHeight: 30
    Layout.maximumWidth: parent.width - 60
    Layout.alignment: Qt.AlignHCenter
    z: 100

    spacing: 0

    Rectangle {
        id: comboBox

        property variant items: []

        property alias selectedItem: chosenItemText.text
        property alias selectedIndex: listView.currentIndex
        signal comboClicked
        Layout.fillWidth: true
        Layout.preferredWidth: parent.width
        Layout.alignment: Qt.AlignHCenter
        height: 30
        smooth: true
        color: Theme.mainBackgroundColor

        Rectangle {
            id: chosenItem
            radius: 4
            width: parent.width
            height: comboBox.height
            color: Theme.mainBackgroundColor
            border.color: Theme.buttonPrimaryColor
            border.width: 2
            smooth: true

            Text {
                anchors.margins: 8
                anchors.fill: parent
                id: chosenItemText
                text: comboBox.items[0]
                color: Theme.buttonPrimaryColor
                font.family: Theme.mainFont
                font.pointSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                smooth: true
            }

                    MouseArea {
                        anchors.fill: parent;
                        onClicked: {
                            comboBox.state = comboBox.state==="dropDown"?"":"dropDown"
                        }
                    }
                }

                Rectangle {
                    id: dropDown
                    width: comboBox.width
                    height: 0
                    clip: true
                    radius: 4
                    anchors.top: chosenItem.bottom
                    anchors.margins: 2
                    color: Theme.mainBackgroundColor
                    border.color: Theme.buttonPrimaryColor
                    border.width: 2

                    ListView {
                        id:listView
                        height: 500
                        model: comboBox.items
                        currentIndex: 0
                        delegate: Item{
                            width: comboBox.width
                            height: comboBox.height

                            Text {
                                text: modelData
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.margins: 5
                                color: Theme.buttonPrimaryColor

                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    comboBox.state = ""
                                    var prevSelection = chosenItemText.text
                                    chosenItemText.text = modelData
                                    if(chosenItemText.text != prevSelection){
                                        comboBox.comboClicked();
                                    }
                                    listView.currentIndex = index;
                                }
                            }
                        }
                    }
                }

                states: State {
                    name: "dropDown";
                    PropertyChanges { target: dropDown; height:40*comboBox.items.length }
                }

                transitions: Transition {
                    NumberAnimation { target: dropDown; properties: "height"; easing.type: Easing.OutExpo; duration: 1000 }
                }
            }
}
