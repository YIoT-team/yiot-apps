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

#ifndef PROVISION_QT_APP_H
#define PROVISION_QT_APP_H

#include <QtCore>
#include <QGuiApplication>

#include <KSQWiFiEnumerator.h>
#include <KSQBLEController.h>

#include <devices/KSQDevices.h>

#include <virgil/iot/qt/VSQIoTKit.h>
#include <yiot-iotkit/netif/KSQUdp.h>

class KSQApplication : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString organizationDisplayName READ organizationDisplayName CONSTANT)
    Q_PROPERTY(QString applicationDisplayName READ applicationDisplayName CONSTANT)
public:
    KSQApplication() = default;
    virtual ~KSQApplication() = default;

    int
    run();

    QString
    organizationDisplayName() const;

    QString
    applicationDisplayName() const;

private:
    KSQWiFiEnumerator m_wifiEnumerator;
    KSQBLEController m_bleController;
    QSharedPointer<KSQUdp> m_netifUdp;

    KSQDevices m_deviceControllers;
};

#endif // PROVISION_QT_APP_H
