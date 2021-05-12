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

#ifndef KSQ_UX_SIMPLIFY_CONTROLLER_H
#define KSQ_UX_SIMPLIFY_CONTROLLER_H

#include <QtCore>
#include <QtNetwork>
#include <QtBluetooth>

#include <QAbstractTableModel>

#include <virgil/iot/qt/VSQIoTKit.h>
#include <devices/KSQDeviceBase.h>

class KSQUXSimplifyController : public QObject {
    Q_OBJECT

public:
    KSQUXSimplifyController();

    KSQUXSimplifyController &
    operator=(KSQUXSimplifyController const &) = delete;

    virtual ~KSQUXSimplifyController() = default;

    Q_INVOKABLE bool
    startDeviceProvision(QString name);

    Q_INVOKABLE bool
    rejectDeviceProvision(QString name);

public slots:
    void
    onBLEDeviceIsClose(QString deviceName, bool requiresProvision);

    void
    onDeviceRequiresProvision(QString deviceName, QSharedPointer<VSQNetifBase> netif, VSQMac deviceMac);

    void
    onNewProvisionedDevice(QSharedPointer<KSQDeviceBase> device);

signals:
    void
    fireRequestDeviceProvision(QString deviceName);

    void
    fireRequestDeviceSetup(KSQDeviceBase *deviceName);

private slots:
    void
    onNewSetup(const VSQMac &mac);

private:
    QSharedPointer<VSQNetifBase> m_netif;
    VSQMac m_deviceMac;
    QSet<QString> m_ignoredDevices;
    QSet<QString> m_provisionedDevices;

    void
    requestProvisionUI(const QString &deviceName);
};

#endif // KSQ_UX_SIMPLIFY_CONTROLLER_H
