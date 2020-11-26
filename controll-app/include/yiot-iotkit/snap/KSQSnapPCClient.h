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

#ifndef _YIOT_QT_SNAP_PC_CLIENT_SERVICE_H_
#define _YIOT_QT_SNAP_PC_CLIENT_SERVICE_H_

#include <QtCore>

#include <common/protocols/snap/pc/pc-structs.h>
#include <common/protocols/snap/pc/pc-client.h>
#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapServiceBase.h>
#include <yiot-iotkit/KSQFeatures.h>

using namespace VirgilIoTKit;

class KSQSnapPCClient final : public QObject, public VSQSingleton<KSQSnapPCClient>, public VSQSnapServiceBase {

    Q_OBJECT

    friend VSQSingleton<KSQSnapPCClient>;

public:
    const VirgilIoTKit::vs_snap_service_t *
    serviceInterface() override {
        return m_snapService;
    }

    // TODO: remove it
    VSQFeatures::EFeature
    serviceFeature() const override {
        return VSQFeatures::SNAP_SNIFFER;
    }

    const QString &
    serviceName() const override {
        static QString name{"LAMP Client"};
        return name;
    }

signals:
    void
    fireStateUpdate(const vs_mac_addr_t mac, const vs_snap_pc_state_t state);

    void
    fireStateError(const vs_mac_addr_t mac);

public slots:
    void
    requestState(const vs_mac_addr_t &mac);

    void
    initPC(const vs_mac_addr_t &mac, const vs_snap_pc_init_t &initData);

private:
    const VirgilIoTKit::vs_snap_service_t *m_snapService;

    KSQSnapPCClient();
    virtual ~KSQSnapPCClient() = default;

    static vs_status_e
    onUpdateState(vs_status_e res, const vs_mac_addr_t *mac, const vs_snap_pc_state_t *data);
};

#endif // _YIOT_QT_SNAP_PC_CLIENT_SERVICE_H_