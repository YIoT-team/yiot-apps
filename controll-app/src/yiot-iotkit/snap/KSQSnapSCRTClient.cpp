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
    VS_LOG_DEBUG(">>> SCRT::INFO");
    QTimer::singleShot(3000, [this]() {
        emit fireInfo();
    });
    return true;
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
KSQSnapSCRTClient::_infoCb(vs_snap_transaction_id_t id, vs_status_e res) {
    VS_LOG_DEBUG("_infoCb");
    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapSCRTClient::_sessionKeyCb(vs_snap_transaction_id_t id, vs_status_e res) {
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
