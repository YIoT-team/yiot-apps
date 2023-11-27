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

import QtQuick 2.15
import QtQuick.Controls.Basic 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

import "qrc:/qml/theme"
import "qrc:/qml/components"

Page {
    property var model
    property string fileName

    id: rotImportPage

    background: Rectangle {
        color: "transparent"
    }

    header: Header {
        id: header
        title: qsTr("Import Root of trust")
        backAction: function() {
            showRoTSettings()
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        //currentFolder: shortcuts.home
        nameFilters: [ "YIoT Root of trust (*.rot)" ]
        onAccepted: {
            var path = fileDialog.fileUrl.toString();
            path= path.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,"");
            fileName = decodeURIComponent(path);
            console.log("You chose: " + fileName)
        }
        onRejected: {
            header.backAction()
        }
    }

    ColumnLayout {
        width: parent.width
        anchors.topMargin: 30
        spacing: 20

        TextField {
            id: password
            placeholderText: qsTr("Password Hint")
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter
            color: Theme.primaryTextColor
            font.pointSize: 14
            leftPadding: 30
            echoMode: TextField.PasswordEchoOnEdit
            background: Rectangle {
                implicitWidth: 200
                implicitHeight: 50
                radius: implicitHeight / 2
                color: "transparent"

                Rectangle {
                    width: parent.width - 10
                    height: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    color: Theme.brandColor
                }
            }
        }

        FormPrimaryButton {
            Layout.bottomMargin: 10
            text: qsTr("Import")
            onClicked: {
                console.log("Start Import root of trust")
                let text = model.object.importData(fileName, password.text)
                showPopupInform(text, function() {
                    showRoTSettings()
                })
            }
        }
    }

    function start(model) {
        rotImportPage.model = model
        fileDialog.visible = true
    }
}
