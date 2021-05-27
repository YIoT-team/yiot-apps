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
import "../../components"
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
                    text: title
                    Layout.fillWidth: true
                }

                Text {
                    text: info.version
                    horizontalAlignment: Text.AlignRight
                    Layout.rightMargin: 15
                    Layout.fillWidth: true
                }
            }

            MouseArea {
                enabled: true
                anchors.fill: parent
                hoverEnabled: true
                anchors.rightMargin: 0
                onClicked: {
                    showPluginPage(model, function() {
                       showSettings()
                       showPluginsList(true)
                    })
                }

                onEntered: {
                    base.color = Theme.contrastBackgroundColor
                }

                onExited: {
                    base.color = "transparent"
                }
            }
        }
        Component.onCompleted: {
            loadAvailablePluginsInfo()
        }
    }

    function loadAvailablePluginsInfo() {
        let path = "https://raw.githubusercontent.com/YIoT-team/yiot-packages/feature/test"
        let lang = "en"
        let jsonData = readFile(path + "/yiot.json")
        var data = JSON.parse(jsonData)
        model.clear()
        var list = data[0]["plugins"]
        for (var i in list) {
            // Helper variables
            let packageUrl = path + "/packages/" + list[i]["dir"] + "/"
            let textsUrl = packageUrl + "texts/" + lang + "/"

            // Logo
            let imageUrl = packageUrl + "logo/logo.png"

            // Texts
            let titleUrl = textsUrl + "title.txt"
            let linkUrl = textsUrl + "link.txt"
            let descriptionUrl = textsUrl + "description.md"

            // General info
            let infoJsonUrl = packageUrl + "/info.json"
            let infoData = JSON.parse(readFile(infoJsonUrl))

            // Fill model data
            model.append({
                             pluginId: i,
                             image: imageUrl,
                             title: readFile(titleUrl),
                             link: readFile(linkUrl),
                             description: readFile(descriptionUrl),
                             info: infoData[0],
                             installed: false
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
