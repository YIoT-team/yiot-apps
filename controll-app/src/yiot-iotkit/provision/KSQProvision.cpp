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

    uint8_t public_key[256] = {0};
    uint16_t public_key_sz = 0;
    vs_secmodule_keypair_type_e keypair_type = VS_KEYPAIR_INVALID;

    // Try to load own public key
    if (VS_CODE_OK == KSQSecModule::instance().secmoduleImpl()->get_pubkey(
                              PRIVATE_KEY_SLOT,
                              public_key,
                              sizeof(public_key),
                              &public_key_sz,
                              &keypair_type
                              )) {
        return true;
    }

    // Try to create own key pair if absent
    if (VS_CODE_OK == KSQSecModule::instance().secmoduleImpl()->create_keypair(
            PRIVATE_KEY_SLOT,
            VS_KEYPAIR_EC_SECP256R1
            )) {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
KSQProvision::~KSQProvision() {
    vs_provision_deinit();
    delete m_storage;
}

//-----------------------------------------------------------------------------
