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

#include <virgil/iot/protocols/snap/scrt/scrt-structs.h>
#include <virgil/iot/protocols/snap/scrt/scrt-client.h>

#include <yiot-iotkit/snap/KSQSnapSCRTClient.h>

using namespace VirgilIoTKit;

//-----------------------------------------------------------------------------
KSQSnapSCRTClient::KSQSnapSCRTClient() {
    vs_snap_scrt_client_service_t impl;
    memset(&impl, 0, sizeof(impl));
    impl.scrt_client_info_cb = _infoCb;
    impl.scrt_client_session_key_cb = _sessionKeyCb;
    impl.scrt_client_add_user_cb = _addUserCb;
    impl.scrt_client_remove_user_cb = _removeUserCb;
    impl.scrt_client_get_users_cb = _getUsersCb;

    m_snapService = vs_snap_scrt_client(impl);
}

//-----------------------------------------------------------------------------
bool
KSQSnapSCRTClient::getInfo(const vs_netif_t *netif, const VSQMac &mac) {
    CHECK_NOT_ZERO_RET(netif, false);
    vs_mac_addr_t _mac = mac;
    return VS_CODE_OK == vs_snap_scrt_get_info(netif, &_mac);
}

//-----------------------------------------------------------------------------
bool
KSQSnapSCRTClient::requestSessionKey(const vs_netif_t *netif, const VSQMac &mac) {
    VS_LOG_DEBUG(">>> SCRT::GSEK");
    QTimer::singleShot(3000, [this]() {
      emit fireSessionKeyReady();
    });
    return false;
}

//-----------------------------------------------------------------------------
bool
KSQSnapSCRTClient::addUser(const vs_netif_t *netif, const VSQMac &mac, const QString &userName) {
    VS_LOG_DEBUG(">>> SCRT::AUSR");
    QTimer::singleShot(3000, [this]() {
      emit fireUserAddDone();
    });
    return false;
}

//-----------------------------------------------------------------------------
bool
KSQSnapSCRTClient::removeUser(const vs_netif_t *netif, const VSQMac &mac, const QString &userName) {
    VS_LOG_DEBUG(">>> SCRT::RUSR");
    QTimer::singleShot(3000, [this]() {
      emit fireUserRemoveDone();
    });
    return false;
}

//-----------------------------------------------------------------------------
bool
KSQSnapSCRTClient::getUsers(const vs_netif_t *netif, const VSQMac &mac) {
    VS_LOG_DEBUG(">>> SCRT::GUSR");
    QTimer::singleShot(3000, [this]() {
      emit fireGetUsersDone();
    });
    return false;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapSCRTClient::_infoCb(vs_snap_transaction_id_t id, vs_status_e res,
                           const vs_scrt_info_response_t* scrt_info) {
    CHECK_NOT_ZERO_RET(scrt_info, VS_CODE_ERR_ZERO_ARGUMENT);

    bool hasProvision = scrt_info->provisioned;
    bool hasOwner = scrt_info->owners_count > 0;
    bool hasOwnerIsYou = false;

    bool keyPresent = scrt_info->own_cert.key_sz;
    KSQPublicKey publicKey;
    if (keyPresent) {
        const vs_pubkey_t *key = (vs_pubkey_t *)scrt_info->own_cert.raw_cert;
        publicKey = KSQPublicKey(*key);
    }

    emit KSQSnapSCRTClient::instance().fireInfo(hasProvision, hasOwner, hasOwnerIsYou, publicKey);
    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapSCRTClient::_sessionKeyCb(vs_snap_transaction_id_t id,
                                 vs_status_e res) {
    VS_LOG_DEBUG("_sessionKeyCb");
    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapSCRTClient::_addUserCb(vs_snap_transaction_id_t id, vs_status_e res) {
    VS_LOG_DEBUG("_addUserCb");
    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapSCRTClient::_removeUserCb(vs_snap_transaction_id_t id, vs_status_e res) {
    VS_LOG_DEBUG("_removeUserCb");
    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapSCRTClient::_getUsersCb(vs_snap_transaction_id_t id, vs_status_e res) {
    VS_LOG_DEBUG("_getUsersCb");
    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
