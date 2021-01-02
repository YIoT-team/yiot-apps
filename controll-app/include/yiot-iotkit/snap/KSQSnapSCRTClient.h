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

#ifndef _YIOT_QT_SNAP_SCRT_CLIENT_SERVICE_H_
#define _YIOT_QT_SNAP_SCRT_CLIENT_SERVICE_H_

#include <QtCore>

#include <virgil/iot/protocols/snap/scrt/scrt-structs.h>
#include <virgil/iot/protocols/snap/scrt/scrt-client.h>
#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapServiceBase.h>

#include <yiot-iotkit/secmodule/KSQPublicKey.h>

using namespace VirgilIoTKit;

class KSQSnapSCRTClient final : public QObject, public VSQSingleton<KSQSnapSCRTClient>, public VSQSnapServiceBase {

    Q_OBJECT

    friend VSQSingleton<KSQSnapSCRTClient>;

public:
    const VirgilIoTKit::vs_snap_service_t *
    serviceInterface() override {
        return m_snapService;
    }

    const QString &
    serviceName() const override {
        static QString name{"SCRT Client"};
        return name;
    }

    bool
    getInfo(const vs_netif_t *netif, const VSQMac &mac);

    bool
    requestSessionKey(const vs_netif_t *netif, const VSQMac &mac);

    bool
    addUser(const vs_netif_t *netif,
            const VSQMac &mac,
            vs_user_type_t type,
            const QString &userName,
            const vs_cert_t &cert);

    bool
    removeUser(const vs_netif_t *netif, const VSQMac &mac, vs_user_type_t type, const QString &userName);

    bool
    getUsers(const vs_netif_t *netif, const VSQMac &mac, vs_user_type_t type);

signals:
    void
    fireInfo(bool hasProvision, bool hasOwner, bool hasOwnerIsYou, const KSQPublicKey &publicKey);
    void
    fireSessionKeyReady();
    void
    fireUserAddDone();
    void
    fireUserRemoveDone();
    void
    fireGetUsersDone();

public slots:

private:
    const VirgilIoTKit::vs_snap_service_t *m_snapService;

    KSQSnapSCRTClient();
    virtual ~KSQSnapSCRTClient() = default;

    static vs_status_e
    _infoCb(vs_snap_transaction_id_t id, vs_status_e res, const vs_scrt_info_response_t *scrt_info);

    static vs_status_e
    _sessionKeyCb(vs_snap_transaction_id_t id, vs_status_e res);

    static vs_status_e
    _addUserCb(vs_snap_transaction_id_t id, vs_status_e res);

    static vs_status_e
    _removeUserCb(vs_snap_transaction_id_t id, vs_status_e res);

    static vs_status_e
    _getUsersCb(vs_snap_transaction_id_t id, vs_status_e res);
};

#endif // _YIOT_QT_SNAP_SCRT_CLIENT_SERVICE_H_
