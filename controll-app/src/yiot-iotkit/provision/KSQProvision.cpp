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

#include <yiot-iotkit/provision/KSQProvision.h>
#include <yiot-iotkit/secmodule/KSQSecModule.h>

#include <virgil/iot/provision/provision.h>
#include <virgil/iot/status_code/status_code.h>
#include <virgil/iot/secmodule/devices/secmodule-soft.h>
#include <virgil/iot/secmodule/secmodule-helpers.h>
#include <virgil/iot/trust_list/trust_list.h>

#include <endian-config.h>

using namespace VirgilIoTKit;

//-----------------------------------------------------------------------------

KSQProvision::KSQProvision() {
    m_valid = false;
    auto base = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation).first();
    m_storage = new KSQStorageFS(base + QDir::separator() + "provision");

    auto storage = m_storage->storageImpl();
    auto secmodule = KSQSecModule::instance().secmoduleImpl();
    vs_provision_events_t events_cb = {NULL};
    if (VS_CODE_OK == vs_provision_init(storage, secmodule, events_cb)) {
        m_valid = true;
    }

    prepareOwnKeyPair();
}

//-----------------------------------------------------------------------------
bool
KSQProvision::prepareOwnKeyPair() {

    bool res = false;
    uint8_t public_key[256] = {0};
    uint16_t public_key_sz = 0;
    vs_secmodule_keypair_type_e keypair_type = VS_KEYPAIR_INVALID;

    // Try to load own public key
    if (VS_CODE_OK == KSQSecModule::instance().secmoduleImpl()->get_pubkey(
                              PRIVATE_KEY_SLOT, public_key, sizeof(public_key), &public_key_sz, &keypair_type)) {
        res = true;
    } else {
        // Try to create own key pair if absent
        if (VS_CODE_OK ==
            KSQSecModule::instance().secmoduleImpl()->create_keypair(PRIVATE_KEY_SLOT, VS_KEYPAIR_EC_SECP256R1)) {
            if (VS_CODE_OK ==
                KSQSecModule::instance().secmoduleImpl()->get_pubkey(
                        PRIVATE_KEY_SLOT, public_key, sizeof(public_key), &public_key_sz, &keypair_type)) {
                res = true;
            }
        }
    }

    if (res) {
        // TODO: VS_KEY_USER_DEVICE
        m_ownPubic = KSQPublicKey(
                keypair_type, QByteArray(reinterpret_cast<char *>(public_key), public_key_sz), VS_KEY_IOT_DEVICE);
    }

    return res;
}

//-----------------------------------------------------------------------------
KSQProvision::~KSQProvision() {
    vs_provision_deinit();
    delete m_storage;
}

//-----------------------------------------------------------------------------
bool
KSQProvision::savePubKey(vs_provision_element_id_e element, const KSQPublicKey &key) {
    return KSQProvision::saveElementData(element, key.datedKey() + key.signature());
}

//-----------------------------------------------------------------------------
bool
KSQProvision::saveElementData(vs_provision_element_id_e element, const QByteArray &data) {
    uint16_t slot;
    CHECK_RET(VS_CODE_OK == vs_provision_get_slot_num((vs_provision_element_id_e)element, &slot),
              false,
              "Unable to get slot");

    auto saveFunc = KSQSecModule::instance().secmoduleImpl()->slot_save;
    CHECK_RET(VS_CODE_OK == saveFunc(static_cast<vs_iot_secmodule_slot_e>(slot),
                                     reinterpret_cast<const uint8_t *>(data.data()),
                                     data.size()),
              false,
              "Unable to get slot");

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQProvision::create(QSharedPointer<KSQRoT> rot) {
    VS_LOG_DEBUG("Creation of self-signed provision");

    CHECK_NOT_ZERO_RET(rot.get(), false);
    CHECK_NOT_ZERO_RET(rot->isValid(), false);
    CHECK_NOT_ZERO_RET(!rot->factory().first.isNull(), false);

    // Save all required public keys
    CHECK_RET(savePubKey(VS_PROVISION_PBR1, rot->recovery1()), false, "Cannot save Recovery 1");
    CHECK_RET(savePubKey(VS_PROVISION_PBR2, rot->recovery2()), false, "Cannot save Recovery 2");
    CHECK_RET(savePubKey(VS_PROVISION_PBA1, rot->auth1()), false, "Cannot save Auth 1");
    CHECK_RET(savePubKey(VS_PROVISION_PBA2, rot->auth2()), false, "Cannot save Auth 2");
    CHECK_RET(savePubKey(VS_PROVISION_PBT1, rot->tl1()), false, "Cannot save TL 1");
    CHECK_RET(savePubKey(VS_PROVISION_PBT2, rot->tl2()), false, "Cannot save TL 2");
    CHECK_RET(savePubKey(VS_PROVISION_PBF1, rot->firmware1()), false, "Cannot save Firmware 1");
    CHECK_RET(savePubKey(VS_PROVISION_PBF2, rot->firmware2()), false, "Cannot save Firmware 2");

    // Sign own public key by factory

    // Need to set time limits manually
    auto signature = KSQSecModule::instance().sign(m_ownPubic.datedKey(), rot->factory());
    CHECK_RET(signature.size(), false, "Cannot sign own public key");

    // Save signature
    CHECK_RET(saveElementData(VS_PROVISION_SGNP, signature), false, "Cannot save own signature");

    // Set TrustList header
    auto tlHeader = rot->trustList().header();
    auto tlHeaderBuf = reinterpret_cast<uint8_t *>(tlHeader.data());
    vs_tl_element_info_t tlHeaderInfo = {.id = VS_TL_ELEMENT_TLH, .index = 0};
    CHECK_RET(
            VS_CODE_OK == vs_tl_save_part(&tlHeaderInfo, tlHeaderBuf, tlHeader.size()), false, "Cannot save TL header");

    // Set TrustList key
    auto keysCnt = rot->trustList().keysCount();
    for (int i = 0; i < keysCnt; i++) {
        auto tlKey = rot->trustList().key(i);
        auto tlKeyBuf = reinterpret_cast<uint8_t *>(tlKey.data());
        vs_tl_element_info_t tlKeyInfo = {.id = VS_TL_ELEMENT_TLC, .index = i};
        CHECK_RET(VS_CODE_OK == vs_tl_save_part(&tlKeyInfo, tlKeyBuf, tlKey.size()), false, "Cannot save TL key");
    }

    // Set TrustList footer
    auto tlFooter = rot->trustList().footer();
    auto tlFooterBuf = reinterpret_cast<uint8_t *>(tlFooter.data());
    vs_tl_element_info_t tlFooterInfo = {.id = VS_TL_ELEMENT_TLF, .index = 0};
    CHECK_RET(
            VS_CODE_OK == vs_tl_save_part(&tlFooterInfo, tlFooterBuf, tlFooter.size()), false, "Cannot save TL footer");

    return true;
}

//-----------------------------------------------------------------------------
void
KSQProvision::onRoTUpdated(QSharedPointer<KSQRoT> rot) {
    create(rot);
}

//-----------------------------------------------------------------------------
