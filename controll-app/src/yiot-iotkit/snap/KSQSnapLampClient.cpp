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

#include <yiot-iotkit/snap/KSQSnapLampClient.h>

using namespace VirgilIoTKit;

//-----------------------------------------------------------------------------
KSQSnapLampClient::KSQSnapLampClient() {
    vs_snap_lamp_client_service_t impl;
    memset(&impl, 0, sizeof(impl));
    impl.device_state_update = &KSQSnapLampClient::onUpdateState;
    m_snapService = vs_snap_lamp_client(impl);
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapLampClient::onUpdateState(vs_status_e res, const vs_mac_addr_t *mac, const vs_snap_lamp_state_t *data) {
    if (VS_CODE_OK == res) {
        emit KSQSnapLampClient::instance().fireStateUpdate(*mac, *data);
    } else {
        emit KSQSnapLampClient::instance().fireStateError(*mac);
    }

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
void
KSQSnapLampClient::requestState(const vs_mac_addr_t &mac) {
    vs_snap_lamp_get_state(vs_snap_netif_routing(), &mac);
}

//-----------------------------------------------------------------------------
void
KSQSnapLampClient::setState(const vs_mac_addr_t &mac, const vs_snap_lamp_state_t &state) {
    qDebug() << ">>> setState: " << (state.is_on ? "ON" : "OFF");
    vs_snap_lamp_set_state(vs_snap_netif_routing(), &mac, &state);
}

//-----------------------------------------------------------------------------
