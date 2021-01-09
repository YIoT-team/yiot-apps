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

#ifndef _YIOT_QT_SNAP_PRVS_CLIENT_SERVICE_H_
#define _YIOT_QT_SNAP_PRVS_CLIENT_SERVICE_H_

#include <QtCore>

#include <virgil/iot/protocols/snap/prvs/prvs-structs.h>
#include <virgil/iot/protocols/snap/prvs/prvs-client.h>
#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapServiceBase.h>

#include <yiot-iotkit/root-of-trust/KSQRoT.h>

using namespace VirgilIoTKit;

class KSQSnapPRVSClient final : public QObject, public VSQSingleton<KSQSnapPRVSClient>, public VSQSnapServiceBase {

    Q_OBJECT

    friend VSQSingleton<KSQSnapPRVSClient>;

public:
    const VirgilIoTKit::vs_snap_service_t *
    serviceInterface() override;

    const QString &
    serviceName() const override;

    bool
    provisionDevice(QSharedPointer<VSQNetifBase> netif, VSQMac deviceMac, QSharedPointer<KSQRoT> rootOfTrust);

signals:
    void
    fireProvisionStateChanged(QString state);
    void
    fireProvisionDone();
    void
    fireProvisionError(QString error);


public slots:

private:
    static const size_t kDefaultTimeoutMs = 7000;
    static const size_t kPubKeyBufMax = 512;
    const VirgilIoTKit::vs_snap_service_t *m_snapService;

    std::condition_variable m_waitCondition;
    std::mutex m_waitGuard;

    static vs_status_e
    _wait(uint32_t waitMs, int *condition, int idle);

    static vs_status_e
    _waitStop(int *condition, int expect);

    KSQSnapPRVSClient();
    virtual ~KSQSnapPRVSClient() = default;

    KSQPublicKey
    getDevicePublicKey(QSharedPointer<VSQNetifBase> netif, VSQMac deviceMac);

    bool
    uploadKeys(QSharedPointer<VSQNetifBase> netif, VSQMac deviceMac, QSharedPointer<KSQRoT> rootOfTrust);

    bool
    uploadData(QSharedPointer<VSQNetifBase> netif,
               VSQMac deviceMac,
               vs_snap_prvs_element_e prvsElement,
               const QByteArray &data);

    bool
    signDevice(QSharedPointer<VSQNetifBase> netif,
               VSQMac deviceMac,
               const KSQPublicKey &deviceKey,
               QSharedPointer<KSQRoT> rootOfTrust);

    bool
    uploadTrustList(QSharedPointer<VSQNetifBase> netif, VSQMac deviceMac, QSharedPointer<KSQRoT> rootOfTrust);
};

#endif // _YIOT_QT_SNAP_PRVS_CLIENT_SERVICE_H_
