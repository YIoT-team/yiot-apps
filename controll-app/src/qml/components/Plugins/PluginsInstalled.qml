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

Item {
    Layout.topMargin: 1
    Layout.leftMargin: 10

    ListView {
        anchors.fill: parent
        model: ListModel { id: model}
        delegate: Rectangle {
            id: base
            width: parent.width
            height: 55
            color: "transparent"

            RowLayout {
                id: listDelegate
                anchors.fill: parent
                clip: true

                Image {
                    id: icon
                    source: (image.indexOf("://") != -1) ? image : "qrc:/qml/resources/icons/%1/%2.png".arg(Theme.state).arg(image)
                    Layout.maximumHeight: listDelegate.height * 0.7
                    Layout.maximumWidth: Layout.maximumHeight
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignLeft
                    Layout.leftMargin: 10
                }

                Text {
                    id: nameText
                    text: title
                    color: Theme.primaryTextColor
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: UiHelper.fixFontSz(14)

                    Layout.alignment: Qt.AlignLeft
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

//                RowLayout {
//                    id: actionsBlock
//                    Layout.rightMargin: 10
//
//                    Image {
//                        id: iconAction
//                        source: "qrc:/qml/resources/icons/Arrow-Right.png"
//                        Layout.maximumHeight: listDelegate.height * 0.7
//                        Layout.maximumWidth: Layout.maximumHeight
//                        fillMode: Image.PreserveAspectFit
//                        Layout.alignment: Qt.AlignRight
//                        Layout.rightMargin: 10
//                    }
//                }
            }
        }
        Component.onCompleted: {
            loadPluginsInfo()
        }
    }

    function loadPluginsInfo() {
        let path = "file:///Users/kutashenko/Work/yiot/yiot-extern"
        let lang = "en"
        let jsonData = readFile(path + "/plugins.json")
        var data = JSON.parse(jsonData)
        model.clear()
        var list = data[0]["plugins"]
        for (var i in list) {
            let imageUrl = path + "/" + list[i]["location"] + "/logo.png"
            let titleUrl = path + "/" + list[i]["location"] + "/texts/" + lang + "/title.txt"
            let versionUrl = path + "/" + list[i]["location"] + "/version.txt"
            model.append({
                             image: imageUrl,
                             title: readFile(titleUrl),
                             version: readFile(versionUrl)
                         })
        }
    }

    function readFile(fileUrl) {
        var request = new XMLHttpRequest()
        request.open("GET", fileUrl, false)
        request.send(null)
        return request.responseText
    }
}
