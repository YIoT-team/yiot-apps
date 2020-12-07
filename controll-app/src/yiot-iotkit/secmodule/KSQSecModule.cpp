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
#include <virgil/iot/converters/crypto_format_converters.h>
#include <virgil/iot/vs-soft-secmodule/vs-soft-secmodule.h>

#include <yiot-iotkit/storages/KSQStorageKeychain.h>


// !!! Include private headers from Software security module !!!
using namespace VirgilIoTKit;
extern "C" {
#include <private/vs-soft-secmodule-internal.h>
}


//-----------------------------------------------------------------------------
KSQSecModule::KSQSecModule() {
    auto storage = KSQStorageKeychain::instance().storageImpl();
    m_secmoduleImpl = vs_soft_secmodule_impl(storage);
}

//-----------------------------------------------------------------------------
KSQKeyPair
KSQSecModule::generateKeypair(vs_secmodule_keypair_type_e keypair_type) const {
    uint8_t public_key[MAX_INTERNAL_PUBKEY_SIZE];
    uint16_t public_key_sz;

    uint8_t private_key[MAX_INTERNAL_PRIVKEY_SIZE];
    uint16_t private_key_sz;

    if (VS_CODE_OK != _keypair_create_internal(keypair_type,
                                              public_key,
                                              sizeof(public_key),
                                              &public_key_sz,
                                              private_key,
                                              sizeof(private_key),
                                              &private_key_sz)) {
        qDebug() << "Unable to create keypair";
        return std::make_pair(nullptr, nullptr);
    }

    if (!vs_converters_pubkey_to_raw(keypair_type,
                                    public_key,
                                    public_key_sz,
                                    public_key,
                                    sizeof(public_key),
                                    &public_key_sz)) {
        qDebug() << "Unable to convert a public key to raw";
        return std::make_pair(nullptr, nullptr);
    }

    auto pubkey = QSharedPointer<KSQPublicKey>::create(keypair_type,
                                                        QByteArray(reinterpret_cast<char *>(public_key), public_key_sz));
    auto privkey = QSharedPointer<KSQPrivateKey>::create(keypair_type,
                                                        QByteArray(reinterpret_cast<char *>(private_key), private_key_sz));

    return std::make_pair(privkey, pubkey);
}

//-----------------------------------------------------------------------------
