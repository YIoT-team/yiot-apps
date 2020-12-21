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

class KSQDeviceSetupController : public QObject, public VSQSingleton<KSQDeviceSetupController> {
    Q_OBJECT

    friend VSQSingleton<KSQDeviceSetupController>;

public:
    bool
    isValid() const {
        return m_valid;
    }

    bool
    start(QSharedPointer<VSQNetifBase> netif, VSQMac deviceMac);

    bool
    configure();

    void
    error(const QString & error);

signals:
    void
    fireStateInfo(QString state);

    void
    fireError(QString text);

    void
    fireFinished(QSharedPointer<VSQNetifBase> m_netif);

    void
    fireInitializationReady();

private slots:
    void
    onDeviceSecurityInfo();

    void
    onDeviceInfo(const VSQDeviceInfo &deviceInfo);

    void
    onConfigurationDone();

    void
    onConfigurationError();

private:
    KSQDeviceSetupController();
    virtual ~KSQDeviceSetupController();

    bool m_valid;
    bool m_readyDeviceInfo;
    bool m_readyDeviceSecurityInfo;

    QSharedPointer<VSQNetifBase> m_netif;
    VSQMac m_deviceMac;

    bool
    checkInitalStep();
};

#endif // _YIOT_QT_DEVICE_SETUP_CONTROLLER_H_
