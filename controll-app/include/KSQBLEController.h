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

signals:
    void
    fireConnected();
    void
    fireDisconnected();
    void
    fireError(QString text);
    void
    fireDataSent();
    void
    fireDataReceived();

public slots:

    Q_INVOKABLE bool
    configureWiFi(const QString &deviceName, const QString &ssid, const QString &password);

private slots:
    void
    onConnected(bool);

    void
    onDisconnected();

    void
    onDeviceError();

    void
    onConfigurationDone();

private:
    VSQNetifBLEEnumerator m_bleEnumerator;
    QSharedPointer<VSQNetifBLE> m_netifBLE;

    bool m_needWiFiConfig;
};

#endif // PROVISION_QT_BLE_CONTROLLER_H
