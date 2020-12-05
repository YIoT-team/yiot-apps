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
#include <yiot-iotkit/secmodule/KSQSecModule.h>

#include <virgil/iot/secmodule/secmodule.h>

#include <yiot-iotkit/storages/KSQStorageKeychain.h>

// !!! Include private headers from Software security module !!!
using namespace VirgilIoTKit;
extern "C" {
#include <private/vs-soft-secmodule-internal.h>
}


//-----------------------------------------------------------------------------
KSQSecModule::KSQSecModule() {
}

//-----------------------------------------------------------------------------
KSQKeyPair
KSQSecModule::generateKeypair(vs_secmodule_keypair_type_e keypair_type) const {

//    vs_status_e
//    _keypair_create_internal(vs_secmodule_keypair_type_e keypair_type,
//                             uint8_t *public_key,
//                             uint16_t pubkey_buf_sz,
//                             uint16_t *public_key_sz,
//                             uint8_t *private_key,
//                             uint16_t prvkey_buf_sz,
//                             uint16_t *private_key_sz);

    return std::make_pair(nullptr, nullptr);
}

//-----------------------------------------------------------------------------
