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

#ifndef _YIOT_QT_DEVICE_SETUP_CONTROLLER_H_
#define _YIOT_QT_DEVICE_SETUP_CONTROLLER_H_

#include <QtCore>

#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/qt/VSQIoTKit.h>
#include <yiot-iotkit/setup/KSQDeviceSetupData.h>

#include <yiot-iotkit/secmodule/KSQPublicKey.h>

using namespace VirgilIoTKit;

class KSQDeviceSetupController : public QObject, public VSQSingleton<KSQDeviceSetupController> {
    Q_OBJECT

    friend VSQSingleton<KSQDeviceSetupController>;

    Q_PROPERTY(QObject *data READ deviceData NOTIFY fireDeviceDataChanged)

public:
    bool
    isValid() const {
        return m_valid;
    }

    bool
    start(QSharedPointer<VSQNetifBase> netif, VSQMac deviceMac);

    Q_INVOKABLE void
    stop();

    Q_INVOKABLE bool
    configure(QString ssid, QString password);

    void
    error(const QString &error);

signals:
    void
    fireStateInfo(QString state);

    void
    fireError(QString text);

    void
    fireFinished(QSharedPointer<VSQNetifBase> m_netif);

    void
    fireInitializationReady();

    void
    fireUploadStarted();

    void
    fireUploadDone();

    void
    fireUploadStopped();

    void
    fireDeviceDataChanged();

private slots:
    void
    onDeviceSecurityInfo(bool hasProvision, bool hasOwner, bool ownerIsYou, const KSQPublicKey &publicKey);

    void
    onDeviceInfo(const VSQDeviceInfo &deviceInfo);

    void
    onConfigurationDone();

    void
    onConfigurationError();

    void
    onProvisionError(QString errorStr);

    void
    onProvisionDone();

    void
    onAddUserError(QString errorStr);

    void
    onAddUserDone();

private:
    KSQDeviceSetupController();
    virtual ~KSQDeviceSetupController();

    bool m_valid;
    bool m_readyDeviceInfo;
    bool m_readyDeviceSecurityInfo;

    KSQDeviceSetupData m_deviceData;

    QSharedPointer<VSQNetifBase> m_netif;
    VSQMac m_deviceMac;

    bool
    checkInitalStep();

    Q_INVOKABLE QObject *
    deviceData();

    void
    done();
};

#endif // _YIOT_QT_DEVICE_SETUP_CONTROLLER_H_
