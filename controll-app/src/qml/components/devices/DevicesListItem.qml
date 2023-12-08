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

Rectangle {
    property bool topLevel: true
    property bool submodulePresent: true
    property bool showControlsPermanent: false
    property string controlElementUrl: ""
    property var action: function() { }

    id: deviceItem
    color: "transparent"
    height: topLevel ? 50 : 40
    width: commonWidth
    visible: true

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
            enabled: !topLevel || (image !== "")
            source: topLevel ? image.arg(Theme.state) : model.js.stateImage(model).arg(Theme.state)
            Layout.maximumHeight: itemText.height * 0.7
            Layout.maximumWidth: Layout.maximumHeight
            Layout.leftMargin: 5
            fillMode: Image.PreserveAspectFit
            Layout.alignment: Qt.AlignLeft

            Connections {
                enabled: !topLevel
                target: topLevel ? null : deviceController
                ignoreUnknownSignals: true
                function onFireStateChanged() {
                    icon.source = model.js.stateImage(model).arg(Theme.state)
                }
            }
        }

        // Name of element
        Label {
            id: categoryText
            text: name
            color: Theme.primaryTextColor
            verticalAlignment: Qt.AlignVCenter
            font.pointSize: 14

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
        property bool collapsed: (topLevel && submodulePresent) ? subModel.collapsed : false
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
            if (submodulePresent) {
                subModel.collapsed = collapsed
            }
        }

        Connections {
            enabled: topLevel
            target: (topLevel && submodulePresent) ? subModel : null
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
            if (!submodulePresent) {
                action()
                return
            }
            if (topLevel) {
                subModel.collapsed = !subModel.collapsed
            } else {
                activateDeviceView(deviceController)
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
            if (loader === null || loader.item === null) {
                return
            }

            var v = loader.item.containsMouse || mainMouseArea.containsMouse
            loader.item.state = (v || showControlsPermanent) ? "visible" : "hidden"
            deviceItem.color = v ? Theme.contrastBackgroundColor : "transparent"
        })
    }

}
