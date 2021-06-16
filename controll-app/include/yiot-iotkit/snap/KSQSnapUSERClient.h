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

#ifndef _YIOT_QT_SNAP_USER_CLIENT_SERVICE_H_
#define _YIOT_QT_SNAP_USER_CLIENT_SERVICE_H_

#include <QtCore>

#include <common/protocols/snap/user/user-structs.h>
#include <common/protocols/snap/user/user-client.h>
#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapServiceBase.h>
#include <yiot-iotkit/KSQFeatures.h>

using namespace VirgilIoTKit;

class KSQSnapUSERClient final : public QObject, public VSQSingleton<KSQSnapUSERClient>, public VSQSnapServiceBase {

    Q_OBJECT

    friend VSQSingleton<KSQSnapUSERClient>;

public:
    const VirgilIoTKit::vs_snap_service_t *
    serviceInterface() override {
        return m_snapService;
    }

    const QString &
    serviceName() const override {
        static QString name{"USER Client"};
        return name;
    }

    bool
    sendCommand(QString mac, QString json);

signals:
    void
    fireStateUpdate(const vs_mac_addr_t mac, QString data);

    void
    fireStateError(const vs_mac_addr_t mac);

public slots:
    void
    requestState(const vs_mac_addr_t &mac);

private:
    const VirgilIoTKit::vs_snap_service_t *m_snapService;

    KSQSnapUSERClient();
    virtual ~KSQSnapUSERClient() = default;

    static vs_status_e
    onUpdateState(vs_status_e res, const vs_mac_addr_t *mac, const char *json);
};

#endif // _YIOT_QT_SNAP_PC_CLIENT_SERVICE_H_
