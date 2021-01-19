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

import "../../theme"
import "../../components/devices"
import "../../components/devices/lamp"

Item {
    property alias model: list.model
    property int w: parent.width

    anchors.fill: parent
    anchors.topMargin: 1

    // Sub elements
    Component {
        id: subItemDelegateComponent
        Column {
            property int commonWidth: w
            property alias model: subItemRepeater.model
            Repeater {
                id: subItemRepeater
                delegate: DevicesListItem {
                    topLevel: false
                    controlElementUrl: deviceActions(deviceType)
                }
            }
        }
    }

    // Main (category) elements
    Component {
        id: categoryDelegate
        Column {
            property int commonWidth: w

            width: commonWidth

            //------------------[FOR TESTING]------------------
            Rectangle {
                id: testDevice
                color: "transparent"
                height: 40
                width: commonWidth

                RowLayout {
                    id: deviceDescription
                    anchors.leftMargin: 1
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: commonWidth - 200
                    clip: true

                    // Device image
                    Image {
                        id: icon
                        source: "qrc:/qml/resources/icons/%1/devices/pc/rpi.png".arg(Theme.state)
                        Layout.maximumHeight: deviceDescription.height * 0.7
                        Layout.maximumWidth: Layout.maximumHeight
                        Layout.leftMargin: 5
                        fillMode: Image.PreserveAspectFit
                        Layout.alignment: Qt.AlignLeft
                    }

                    // Name of element
                    Label {
                        id: deviceNameText
                        text: "TEST-RPi"
                        color: Theme.primaryTextColor
                        verticalAlignment: Qt.AlignVCenter
                        font.pointSize: UiHelper.fixFontSz(14)

                        Layout.alignment: Qt.AlignLeft
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.rightMargin: 5
                    }
                }

                // Loader for current element
                Loader {
                    id: loader
                    onLoaded: {
                        z = 1
                        anchors.left = deviceDescription.right
                        anchors.right = parent.right
                        anchors.top = parent.top
                        anchors.bottom = parent.bottom
                    }
                 }

                // Catch mause events
                MouseArea {
                    id: mainMouseArea
                    z: 0
                    enabled: true
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        showRPiSettings()
                    }

                    onEntered: {
                        testDevice.color = Theme.contrastBackgroundColor
                    }

                    onExited: {
                        testDevice.color = "transparent"
                    }
                }
            }
            //-------------------------------------------------

            DevicesListItem {
                showControlsPermanent: true
                controlElementUrl: deviceCategoryActions(categoryType)
            }

            Loader {
                id: subItemLoader

                visible: !subModel.collapsed
                property variant subItemModel : subModel
                sourceComponent: subModel.collapsed ? null : subItemDelegateComponent
                onStatusChanged: if (status == Loader.Ready) item.model = subItemModel
            }
        }
    }

    // List container
    ListView {
        id: list
        anchors.fill: parent
        highlightFollowsCurrentItem: false
        delegate: categoryDelegate
    }
}
