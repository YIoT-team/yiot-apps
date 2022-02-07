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

import "qrc:/qml/theme"
import "qrc:/qml/components"
import "qrc:/qml/components/devices"
import "qrc:/qml/pages/settings/extensions"

SwipeView {
    id: servicesSwipeView
    objectName: "serviceControlContainer"
    interactive: false

    property int serviceIndex: 0

    ExtLoginPage {
        id: loginPage
    }

    function show(serviceController) {
        servicesSwipeView.serviceIndex = serviceController.js.controlPageIdx
        if(serviceController.js.needLogin == true && settings.getServicesLogin() == "") {
            servicesSwipeView.currentIndex = 0
            loginPage.visible = true
        } else {
            console.log(">>> Service login: " + settings.getServicesLogin())
            console.log(">>> Service password: " + settings.getServicesPass())
            showServicePage()
        }
    }

    function showServicePage() {
        servicesSwipeView.currentIndex = servicesSwipeView.serviceIndex
        for (var i = 0; i < servicesSwipeView.count; ++i) {
            var item = servicesSwipeView.itemAt(i)
            item.visible = i == servicesSwipeView.currentIndex
        }
    }

    function addServiceControl(qmlFile, controller) {
        var component = Qt.createComponent(qmlFile);
        var controlPage = component.createObject(servicesSwipeView);

        if (controlPage == null) {
            console.log("Error creating object")
        }

        servicesSwipeView.addItem(controlPage)
        controlPage.controller = controller
        return servicesSwipeView.count - 1
    }
}
