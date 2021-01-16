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

#include <yiot-iotkit/snap/KSQSnapPCClient.h>

using namespace VirgilIoTKit;

//-----------------------------------------------------------------------------
KSQSnapPCClient::KSQSnapPCClient() {
    vs_snap_pc_client_service_t impl;
    memset(&impl, 0, sizeof(impl));
    // impl.device_state_update = &KSQSnapPCClient::onUpdateState;
    m_snapService = vs_snap_pc_client(impl);
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapPCClient::onUpdateState(vs_status_e res, const vs_mac_addr_t *mac /*, const vs_snap_pc_state_t *data*/) {
    if (VS_CODE_OK == res) {
        // emit KSQSnapPCClient::instance().fireStateUpdate(*mac, *data);
    } else {
        emit KSQSnapPCClient::instance().fireStateError(*mac);
    }

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
void
KSQSnapPCClient::requestState(const vs_mac_addr_t &mac) {
    vs_snap_pc_get_state(vs_snap_netif_routing(), &mac);
}

//-----------------------------------------------------------------------------
void
KSQSnapPCClient::initPC(const vs_mac_addr_t &mac /*, const vs_snap_pc_init_ssh_t &initData*/) {
    // vs_snap_pc_init_ssh(vs_snap_netif_routing(), &mac, &initData);
}

//-----------------------------------------------------------------------------


bool
KSQSnapPCClient::sendCommand(QString mac, QString json) {
    qDebug() << "MAC: " << mac << " JSON: " << json;
    return false;
}

//-----------------------------------------------------------------------------