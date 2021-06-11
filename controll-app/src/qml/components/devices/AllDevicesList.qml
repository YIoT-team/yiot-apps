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
