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

#include <virgil/iot/qt/VSQIoTKit.h>
#include <cstring>

#include <yiot-iotkit/snap/KSQSnapUSERClient.h>

using namespace VirgilIoTKit;

//-----------------------------------------------------------------------------
KSQSnapUSERClient::KSQSnapUSERClient() {
    vs_snap_user_client_service_t impl;
    memset(&impl, 0, sizeof(impl));
    impl.device_state_update = &KSQSnapUSERClient::onUpdateState;
    m_snapService = vs_snap_user_client(impl);
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapUSERClient::onUpdateState(vs_status_e res, const vs_mac_addr_t *mac, const char *json) {
    if (VS_CODE_OK == res && json) {
        emit KSQSnapUSERClient::instance().fireStateUpdate(*mac, QString::fromStdString(json));
    } else {
        emit KSQSnapUSERClient::instance().fireStateError(*mac);
    }

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
void
KSQSnapUSERClient::requestState(const vs_mac_addr_t &mac) {
    vs_snap_user_get_state(vs_snap_netif_routing(), &mac);
}

//-----------------------------------------------------------------------------
bool
KSQSnapUSERClient::sendCommand(QString mac, QString json) {
    auto macVal = VSQMac(mac);

    if (macVal == invalidMac) {
        VS_LOG_WARNING("Cannot send command to an invalid MAC address");
        return false;
    }

    vs_mac_addr_t macLowLevel = macVal;
    return VS_CODE_OK == vs_snap_user_command(vs_snap_netif_routing(), &macLowLevel, json.toStdString().c_str());
}

//-----------------------------------------------------------------------------
