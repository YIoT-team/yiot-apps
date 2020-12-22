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

Rectangle {
    property bool topLevel: true
    property bool showControlsPermanent: false
    property string controlElementUrl: ""

    id: deviceItem
    color: "transparent"
    height: topLevel ? 50 : 40
    width: commonWidth

    RowLayout {
        id: itemText
        anchors.leftMargin: topLevel ? 0 : 30
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: commonWidth - 200
        clip: true

        // Device state or Category image
        Image {
            id: icon
            visible: true
            enabled: visible
            source: "qrc:/qml/resources/icons/%1/%2.png".arg(Theme.state).arg(topLevel ? image : deviceStateImage(model))
            Layout.maximumHeight: itemText.height * 0.7
            Layout.maximumWidth: Layout.maximumHeight
            Layout.leftMargin: 5
            fillMode: Image.PreserveAspectFit
            Layout.alignment: Qt.AlignLeft

            Connections {
                enabled: !topLevel
                target: topLevel ? null : deviceController
                function onFireStateChanged() {
                    icon.source = "qrc:/qml/resources/icons/%1/%2.png".arg(Theme.state).arg(deviceStateImage(model))
                }
            }
        }

        // Name of element
        Label {
            id: categoryText
            text: name
            color: Theme.primaryTextColor
            verticalAlignment: Qt.AlignVCenter
            font.pointSize: UiHelper.fixFontSz(14)

            Layout.alignment: Qt.AlignLeft
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.rightMargin: 5
        }
    }

    // Loader of actions panel for current element
    Loader {
        id: loader
        property variant modelData: model
        property bool collapsed: topLevel ? subModel.collapsed : false
        source: controlElementUrl
        onLoaded: {
            z = 1
            anchors.left = itemText.right
            anchors.right = parent.right
            anchors.top = parent.top
            anchors.bottom = parent.bottom
            checkActivity()
            item.onContainsMouseChanged.connect(function() {
                checkActivity()
            })
        }
        onCollapsedChanged: {
            subModel.collapsed = collapsed
        }

        Connections {
            enabled: topLevel
            target: topLevel ? subModel : null
            function onCollapsedChanged() {
                loader.collapsed = subModel.collapsed
            }
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
            if (topLevel) {
                subModel.collapsed = !subModel.collapsed
            } else {
                activateDeviceView(deviceType, name, deviceController)
            }
        }

        onEntered: {
            list.currentIndex = index
            checkActivity()
        }

        onExited: {
            checkActivity()
        }
    }

    // Creation of Timer objects
    function timerObj() {
        return Qt.createQmlObject("import QtQuick 2.0; Timer {}", list)
    }

    // Delay functions with callback
    function delay(delayTime, cb) {
        var timer = new timerObj()
        timer.interval = delayTime
        timer.repeat = false
        timer.triggered.connect(cb)
        timer.start()
    }

    // Delayed check of view state to smooth moving between elements
    function checkActivity() {
        delay(5, function() {
            var v = loader.item.containsMouse || mainMouseArea.containsMouse
            loader.item.state = (v || showControlsPermanent) ? "visible" : "hidden"
            deviceItem.color = v ? Theme.contrastBackgroundColor : "transparent"
        })
    }

}
