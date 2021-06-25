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

#include <controllers/KSQBLEController.h>
#include <controllers/KSQBlankDevicesController.h>
#include <controllers/KSQUXSimplifyController.h>
#include <controllers/devices/KSQAllDevicesController.h>
#include <controllers/integrations/KSQIntegrationsController.h>

#include <controllers/extensions/KSQExtensions.h>

#include <virgil/iot/qt/VSQIoTKit.h>

#include <yiot-iotkit/netif/KSQUdp.h>
#include <yiot-iotkit/netif/KSQNetifWebsocket.h>
#include <yiot-iotkit/root-of-trust/KSQRoTController.h>

class KSQApplication : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString organizationDisplayName READ organizationDisplayName CONSTANT)
    Q_PROPERTY(QString applicationVersion READ applicationVersion CONSTANT)
    Q_PROPERTY(QString applicationDisplayName READ applicationDisplayName CONSTANT)
public:
    KSQApplication() = default;
    virtual ~KSQApplication() = default;

    int
    run();

    QString
    organizationDisplayName() const;

    QString
    applicationVersion() const;

    QString
    applicationDisplayName() const;

public slots:
    void
    onProvisionDone(QString mac);

    void
    onIntegrationActivate(QString integrationId, QString message);

    void
    onIntegrationDeactivate(QString integrationId);

    Q_INVOKABLE void
    updateDevices();

private:
    KSQWiFiEnumerator m_wifiEnumerator;
    QSharedPointer<KSQBLEController> m_bleController;
    QSharedPointer<KSQBlankDevicesController> m_localBlankDevicesController;
    QSharedPointer<KSQUXSimplifyController> m_uxController;
    QSharedPointer<KSQUdp> m_netifUdp;
    QSharedPointer<KSQNetifWebsocket> m_netifWebsock;

    QSharedPointer<KSQAllDevicesController> m_deviceControllers;
    QSharedPointer<KSQIntegrationsController> m_integrations;
    QSharedPointer<KSQExtensions> m_extensionDevices;
    QSharedPointer<KSQExtensions> m_extensionIntegrations;
    QSharedPointer<KSQExtensions> m_extensionPlugins;

    static const QString kWebSocketIntegrationID;
};

#endif // PROVISION_QT_APP_H
