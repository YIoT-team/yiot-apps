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

    const QString &
    serviceName() const override {
        static QString name{"LAMP Client"};
        return name;
    }

    bool
    sendCommand(QString mac, QString json);

signals:
    void
    fireStateUpdate(const vs_mac_addr_t mac, const vs_snap_pc_state_t state);

    void
    fireStateError(const vs_mac_addr_t mac);

public slots:
    void
    requestState(const vs_mac_addr_t &mac);

private:
    static const QString kKeyInet;
    static const QString kKeyWiFi;
    static const QString kKeyIPv4WiFi;
    static const QString kKeyIPv4Eth0;

    static const QString kWiFiAP;
    static const QString kWiFiSTA;

    const VirgilIoTKit::vs_snap_service_t *m_snapService;

    KSQSnapPCClient();
    virtual ~KSQSnapPCClient() = default;

    static vs_status_e
    onUpdateState(vs_status_e res, const vs_mac_addr_t *mac, const char *json);

    static bool
    getBool(const QJsonObject &obj, const QString &field);

    static vs_snap_pc_wifi_mode_e
    getWiFiType(const QJsonObject &obj, const QString &field);

    static uint32_t
    getIPv4(const QJsonObject &obj, const QString &field);
};

#endif // _YIOT_QT_SNAP_PC_CLIENT_SERVICE_H_
