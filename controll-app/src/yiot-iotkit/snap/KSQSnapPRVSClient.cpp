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

#include <virgil/iot/protocols/snap/prvs/prvs-structs.h>
#include <virgil/iot/protocols/snap/prvs/prvs-client.h>
#include <virgil/iot/secmodule/secmodule-helpers.h>

#include <yiot-iotkit/snap/KSQSnapPRVSClient.h>
#include <yiot-iotkit/secmodule/KSQSecModule.h>


using namespace VirgilIoTKit;

//-----------------------------------------------------------------------------
KSQSnapPRVSClient::KSQSnapPRVSClient() {
    vs_snap_prvs_client_impl_t impl;
    memset(&impl, 0, sizeof(impl));
    m_snapService = vs_snap_prvs_client(impl);
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
KSQSnapPRVSClient::getDevicePublicKey(const vs_netif_t *netif,
                                      const vs_mac_addr_t *deviceMac) {
    QByteArray pubKey;
    pubKey.resize(sizeof(vs_pubkey_t) + kPubKeyBufMax);

    if (VS_CODE_OK != vs_snap_prvs_save_provision(netif,
                                                  deviceMac,
                                                  reinterpret_cast<uint8_t*> (pubKey.data()),
                                                  pubKey.size(),
                                                  kDefaultTimeoutMs)) {
        VS_LOG_ERROR("Cannot get public key of device");
        return KSQPublicKey();
    }

    const auto _key = reinterpret_cast<vs_pubkey_t*> (pubKey.data());
    auto ecType = static_cast<vs_secmodule_keypair_type_e> (_key->ec_type);
    auto keyData = QByteArray::fromRawData(reinterpret_cast<char *> (_key->meta_and_pubkey + _key->meta_data_sz),
                                           vs_secmodule_get_pubkey_len(ecType));
    auto provisionType = static_cast<vs_key_type_e> (_key->key_type);
    return KSQPublicKey(ecType, keyData, provisionType);
}

//-----------------------------------------------------------------------------
bool
KSQSnapPRVSClient::uploadData(const vs_netif_t *netif,
                              const vs_mac_addr_t *deviceMac,
                              vs_snap_prvs_element_e prvsElement,
                              const QByteArray &data) {
    CHECK_NOT_ZERO_RET(netif, false);
    CHECK_NOT_ZERO_RET(deviceMac, false);

    if (VS_CODE_OK != vs_snap_prvs_set(netif,
                                       deviceMac,
                                       prvsElement,
                                       reinterpret_cast<const uint8_t*> (data.data()),
                                       data.size(),
                                       kDefaultTimeoutMs)) {
        VS_LOG_ERROR("Cannot get upload data to device");
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool
KSQSnapPRVSClient::uploadKeys(const vs_netif_t *netif,
                              const vs_mac_addr_t *deviceMac,
                              QSharedPointer<KSQRoT> rootOfTrust) {
    emit fireProvisionStateChanged(tr("Upload Recovery Key 1"));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBR1, rootOfTrust->recovery1().datedKey())) {
        emit fireProvisionError(tr("Cannot upload Recovery Key 1"));
        return false;
    }

    emit fireProvisionStateChanged(tr("Upload Recovery Key 2"));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBR2, rootOfTrust->recovery2().datedKey())) {
        emit fireProvisionError(tr("Cannot upload Recovery Key 2"));
        return false;
    }

    emit fireProvisionStateChanged(tr("Upload Auth Key 1"));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBA1, rootOfTrust->auth1().datedKey())) {
        emit fireProvisionError(tr("Cannot upload Auth Key 1"));
        return false;
    }

    emit fireProvisionStateChanged(tr("Upload Auth Key 2"));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBA2, rootOfTrust->auth2().datedKey())) {
        emit fireProvisionError(tr("Cannot upload Auth Key 2"));
        return false;
    }

    emit fireProvisionStateChanged(tr("Upload TL Key 1"));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBT1, rootOfTrust->tl1().datedKey())) {
        emit fireProvisionError(tr("Cannot upload TL Key 1"));
        return false;
    }

    emit fireProvisionStateChanged(tr("Upload TL Key 2"));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBT2, rootOfTrust->tl2().datedKey())) {
        emit fireProvisionError(tr("Cannot upload TL Key 2"));
        return false;
    }

    emit fireProvisionStateChanged(tr("Upload Firmware Key 1"));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBF1, rootOfTrust->firmware1().datedKey())) {
        emit fireProvisionError(tr("Cannot upload Firmware Key 1"));
        return false;
    }

    emit fireProvisionStateChanged(tr("Upload Firmware Key 2"));
    if (!uploadData(netif, deviceMac, VS_PRVS_PBF2, rootOfTrust->firmware2().datedKey())) {
        emit fireProvisionError(tr("Cannot upload Firmware Key 2"));
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQSnapPRVSClient::signDevice(const vs_netif_t *netif,
                              const vs_mac_addr_t *deviceMac,
                              const KSQPublicKey &deviceKey,
                              QSharedPointer<KSQRoT> rootOfTrust) {

    // Check input parameters
    if (!netif
        || !deviceMac
        || !deviceKey.isValid()
        || rootOfTrust->factory().first.isNull()
        || rootOfTrust->factory().second.isNull()) {
        emit fireProvisionError(tr("Wrong parameters to sign devices"));
        return false;
    }

    auto signature = KSQSecModule::instance().sign(deviceKey.datedKey(), rootOfTrust->factory());
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
KSQSnapPRVSClient::uploadTrustList(const vs_netif_t *netif,
                                   const vs_mac_addr_t *deviceMac,
                                   QSharedPointer<KSQRoT> rootOfTrust) {

    // Check input parameters
    if (!netif
        || !deviceMac
        || !rootOfTrust->trustList().isValid()
        || !rootOfTrust->trustList().keysCount()) {
        emit fireProvisionError(tr("Wrong parameters to upload TrustList"));
        return false;
    }

    emit fireProvisionStateChanged(tr("Upload TrustList header"));
    auto tlHeader = rootOfTrust->trustList().header();
    if (VS_CODE_OK != vs_snap_prvs_set_tl_header(
                              netif,
                              deviceMac,
                              reinterpret_cast<const uint8_t*> (tlHeader.data()),
                              tlHeader.size(),
                              kDefaultTimeoutMs)) {
        emit fireProvisionError(tr("Cannot upload TrustList header"));
        return false;
    }

    for (int i = 0; i < rootOfTrust->trustList().keysCount(); i++) {
        emit fireProvisionStateChanged(tr("Upload TrustList key %1").arg(i));
        auto tlKey = rootOfTrust->trustList().key(i);
        if (!uploadData(netif, deviceMac, VS_PRVS_TLC, tlKey)) {
            emit fireProvisionError(tr("Cannot upload TrustList key"));
            return false;
        }
    }

    emit fireProvisionStateChanged(tr("Upload TrustList footer"));
    auto tlFooter = rootOfTrust->trustList().footer();
    if (VS_CODE_OK != vs_snap_prvs_set_tl_footer(
            netif,
            deviceMac,
            reinterpret_cast<const uint8_t*> (tlFooter.data()),
            tlFooter.size(),
            kDefaultTimeoutMs * 10)) {
        emit fireProvisionError(tr("Cannot upload TrustList footer"));
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
void
KSQSnapPRVSClient::provisionDevice(const vs_netif_t *netif,
                                   const vs_mac_addr_t *deviceMac,
                                   QSharedPointer<KSQRoT> rootOfTrust) {
    // Prevent broadcasting and routing for network interfaces
    if (!netif || netif == vs_snap_netif_routing()) {
        emit fireProvisionError(tr("Wrong network Interface"));
        return;
    }

    // Prevent usage of broadcast addresses
    if (deviceMac && vs_snap_is_broadcast(deviceMac)) {
        emit fireProvisionError(tr("Provision couldn't be broadcast"));
        return;
    }

    // Check Root of trust is valid
    if (!rootOfTrust->isValid()) {
        emit fireProvisionError(tr("Wrong root of trust for provision"));
        return;
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

    });

    t.detach();
}

//-----------------------------------------------------------------------------
