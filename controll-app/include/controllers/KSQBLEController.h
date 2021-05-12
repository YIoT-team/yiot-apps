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

#ifndef PROVISION_QT_BLE_CONTROLLER_H
#define PROVISION_QT_BLE_CONTROLLER_H

#include <QtCore>

#include <virgil/iot/qt/netif/VSQNetifBLEEnumerator.h>
#include <virgil/iot/qt/netif/VSQNetifBLE.h>

class KSQBLEController : public QObject {
    Q_OBJECT
public:
    KSQBLEController();
    virtual ~KSQBLEController();

    QSharedPointer<VSQNetifBLE>
    netif();

    VSQNetifBLEEnumerator *
    model();

    bool
    isProvisionRequired(const QString &deviceName);

signals:

public slots:

    Q_INVOKABLE bool
    connectDevice(const QString &mac);

private slots:
    void
    onConnected(bool);

    void
    onDisconnected();

    void
    onDeviceError();

    void
    onSetupFinished(QSharedPointer<VSQNetifBase> netif);

private:
    VSQNetifBLEEnumerator m_bleEnumerator;
    QSharedPointer<VSQNetifBLE> m_netifBLE;

    bool m_needWiFiConfig;

    void
    cleanConnections();
};

#endif // PROVISION_QT_BLE_CONTROLLER_H
