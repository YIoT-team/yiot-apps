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
#include <thread>

#include <virgil/iot/protocols/snap/prvs/prvs-structs.h>
#include <virgil/iot/protocols/snap/prvs/prvs-client.h>
#include <virgil/iot/secmodule/secmodule-helpers.h>

#include <yiot-iotkit/snap/KSQSnapPRVSClient.h>
#include <yiot-iotkit/secmodule/KSQSecModule.h>


using namespace VirgilIoTKit;
using namespace std::chrono_literals;

//-----------------------------------------------------------------------------
KSQSnapPRVSClient::KSQSnapPRVSClient() {
    vs_snap_prvs_client_impl_t impl;
    memset(&impl, 0, sizeof(impl));
    impl.wait_func = _wait;
    impl.stop_wait_func = _waitStop;
    m_snapService = vs_snap_prvs_client(impl);
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapPRVSClient::_wait(uint32_t waitMs, int *condition, int idle) {
    // Wait for expected condition
    std::unique_lock<std::mutex> lk(KSQSnapPRVSClient::instance().m_waitGuard);
    KSQSnapPRVSClient::instance().m_waitCondition.wait_for(
            lk, waitMs * 1ms, [condition, idle]() { return *condition != idle; });

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapPRVSClient::_waitStop(int *condition, int expect) {
    {
        std::lock_guard<std::mutex> lock(KSQSnapPRVSClient::instance().m_waitGuard);
        *condition = expect;
    }
    KSQSnapPRVSClient::instance().m_waitCondition.notify_all();
    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
const vs_snap_service_t *
KSQSnapPRVSClient::serviceInterface() {
    return m_snapService;
}

//-----------------------------------------------------------------------------
const QString &
KSQSnapPRVSClient::serviceName() const {
    static QString name{"PRVS Client"};
    return name;
}

//-----------------------------------------------------------------------------
KSQPublicKey
KSQSnapPRVSClient::getDevicePublicKey(VSQNetifBase *netif, VSQMac deviceMac) {
    QByteArray pubKey;
    pubKey.resize(sizeof(vs_pubkey_dated_t) + kPubKeyBufMax);

    vs_mac_addr_t mac = deviceMac;
    if (VS_CODE_OK != vs_snap_prvs_save_provision(!netif ? vs_snap_netif_routing() : netif->lowLevelNetif(),
                                                  &mac,
                                                  reinterpret_cast<uint8_t *>(pubKey.data()),
                                                  pubKey.size(),
                                                  kDefaultTimeoutMs)) {
        VS_LOG_ERROR("Cannot get public key of device");
        return KSQPublicKey();
    }

    const auto _key = reinterpret_cast<vs_pubkey_dated_t *>(pubKey.data());
    return KSQPublicKey(*_key);
}

//-----------------------------------------------------------------------------
bool
KSQSnapPRVSClient::uploadData(VSQNetifBase *netif,
                              VSQMac deviceMac,
                              vs_snap_prvs_element_e prvsElement,
                              const QByteArray &data) {
    vs_mac_addr_t mac = deviceMac;
    if (VS_CODE_OK != vs_snap_prvs_set(!netif ? vs_snap_netif_routing() : netif->lowLevelNetif(),
                                       &mac,
                                       prvsElement,
                                       reinterpret_cast<const uint8_t *>(data.data()),
                                       data.size(),
                                       kDefaultTimeoutMs)) {
        VS_LOG_ERROR("Cannot get upload data to device");
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool
KSQSnapPRVSClient::uploadKeys(VSQNetifBase *netif,
                              VSQMac deviceMac,
                              QSharedPointer<KSQRoT> rootOfTrust) {
    QString prefix(tr("Security provision "));
    int pos = 1;
    emit fireProvisionStateChanged(prefix + QString("%1/8").arg(pos++));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBR1, rootOfTrust->recovery1().signedDatedKey())) {
        emit fireProvisionError(tr("Cannot upload Recovery Key 1"));
        return false;
    }

    emit fireProvisionStateChanged(prefix + QString("%1/8").arg(pos++));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBR2, rootOfTrust->recovery2().signedDatedKey())) {
        emit fireProvisionError(tr("Cannot upload Recovery Key 2"));
        return false;
    }

    emit fireProvisionStateChanged(prefix + QString("%1/8").arg(pos++));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBA1, rootOfTrust->auth1().signedDatedKey())) {
        emit fireProvisionError(tr("Cannot upload Auth Key 1"));
        return false;
    }

    emit fireProvisionStateChanged(prefix + QString("%1/8").arg(pos++));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBA2, rootOfTrust->auth2().signedDatedKey())) {
        emit fireProvisionError(tr("Cannot upload Auth Key 2"));
        return false;
    }

    emit fireProvisionStateChanged(prefix + QString("%1/8").arg(pos++));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBT1, rootOfTrust->tl1().signedDatedKey())) {
        emit fireProvisionError(tr("Cannot upload TL Key 1"));
        return false;
    }

    emit fireProvisionStateChanged(prefix + QString("%1/8").arg(pos++));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBT2, rootOfTrust->tl2().signedDatedKey())) {
        emit fireProvisionError(tr("Cannot upload TL Key 2"));
        return false;
    }

    emit fireProvisionStateChanged(prefix + QString("%1/8").arg(pos++));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBF1, rootOfTrust->firmware1().signedDatedKey())) {
        emit fireProvisionError(tr("Cannot upload Firmware Key 1"));
        return false;
    }

    emit fireProvisionStateChanged(prefix + QString("%1/8").arg(pos++));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBF2, rootOfTrust->firmware2().signedDatedKey())) {
        emit fireProvisionError(tr("Cannot upload Firmware Key 2"));
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQSnapPRVSClient::signDevice(VSQNetifBase *netif,
                              VSQMac deviceMac,
                              const KSQPublicKey &deviceKey,
                              QSharedPointer<KSQRoT> rootOfTrust) {

    // Check input parameters
    if (!deviceMac || !deviceKey.isValid() || rootOfTrust->factory().first.isNull() ||
        rootOfTrust->factory().second.isNull()) {
        emit fireProvisionError(tr("Wrong parameters to sign devices"));
        return false;
    }

    // TODO: Sign vs_pubkey_dated_t
    // auto signature = KSQSecModule::instance().sign(deviceKey.datedKey(), rootOfTrust->factory());
    auto signature = KSQSecModule::instance().sign(deviceKey.val(), rootOfTrust->factory());
    if (signature.isEmpty()) {
        emit fireProvisionError(tr("Cannot sign device's key"));
        return false;
    }

    if (!uploadData(netif, deviceMac, VS_PRVS_SGNP, signature)) {
        emit fireProvisionError(tr("Cannot upload Device signature"));
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQSnapPRVSClient::uploadTrustList(VSQNetifBase *netif,
                                   VSQMac deviceMac,
                                   QSharedPointer<KSQRoT> rootOfTrust) {

    // Check input parameters
    if (!rootOfTrust->trustList().isValid() || !rootOfTrust->trustList().keysCount()) {
        emit fireProvisionError(tr("Wrong parameters to upload TrustList"));
        return false;
    }

    vs_mac_addr_t mac = deviceMac;
    auto lowLevelNetif = !netif ? vs_snap_netif_routing() : netif->lowLevelNetif();

    QString prefix(tr("Upload TrustList  "));
    int pos = 1;
    int cnt = 2 + rootOfTrust->trustList().keysCount();

    emit fireProvisionStateChanged(prefix + QString("%1/%2").arg(pos++).arg(cnt));
    auto tlHeader = rootOfTrust->trustList().header();
    if (VS_CODE_OK != vs_snap_prvs_set_tl_header(lowLevelNetif,
                                                 &mac,
                                                 reinterpret_cast<const uint8_t *>(tlHeader.data()),
                                                 tlHeader.size(),
                                                 kDefaultTimeoutMs)) {
        emit fireProvisionError(tr("Cannot upload TrustList header"));
        return false;
    }

    for (int i = 0; i < rootOfTrust->trustList().keysCount(); i++) {
        emit fireProvisionStateChanged(prefix + QString("%1/%2").arg(pos++).arg(cnt));
        auto tlKey = rootOfTrust->trustList().key(i);
        if (!uploadData(netif, deviceMac, VS_PRVS_TLC, tlKey)) {
            emit fireProvisionError(tr("Cannot upload TrustList key"));
            return false;
        }
    }

    emit fireProvisionStateChanged(prefix + QString("%1/%2").arg(pos++).arg(cnt));
    auto tlFooter = rootOfTrust->trustList().footer();
    if (VS_CODE_OK != vs_snap_prvs_set_tl_footer(lowLevelNetif,
                                                 &mac,
                                                 reinterpret_cast<const uint8_t *>(tlFooter.data()),
                                                 tlFooter.size(),
                                                 kDefaultTimeoutMs * 10)) {
        emit fireProvisionError(tr("Cannot upload TrustList footer"));
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQSnapPRVSClient::provisionDevice(VSQNetifBase *netif,
                                   VSQMac deviceMac,
                                   QSharedPointer<KSQRoT> rootOfTrust) {

    // Check Root of trust is valid
    if (!rootOfTrust->isValid()) {
        emit fireProvisionError(tr("Wrong root of trust for provision"));
        return false;
    }

    // Start provision thread
    std::thread t([this, netif, deviceMac, rootOfTrust]() {
        emit fireProvisionStateChanged(tr("Get public key from device"));
        auto deviceKey = KSQSnapPRVSClient::getDevicePublicKey(netif, deviceMac);
        if (!deviceKey.isValid()) {
            emit fireProvisionError(tr("Cannot get public key from device"));
            return;
        }

        if (!uploadKeys(netif, deviceMac, rootOfTrust)) {
            return;
        }

        emit fireProvisionStateChanged(tr("Sign device"));
        if (!signDevice(netif, deviceMac, deviceKey, rootOfTrust)) {
            emit fireProvisionError(tr("Cannot sign device"));
            return;
        }

        if (!uploadTrustList(netif, deviceMac, rootOfTrust)) {
            return;
        }

        emit fireProvisionDone();
    });

    t.detach();

    return true;
}

//-----------------------------------------------------------------------------
