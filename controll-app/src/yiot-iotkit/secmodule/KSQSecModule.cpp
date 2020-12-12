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
#include <virgil/iot/secmodule/secmodule-helpers.h>
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
QByteArray
KSQSecModule::sign(const QByteArray &data,
                   QSharedPointer<KSQPrivateKey> key,
                   vs_secmodule_hash_type_e hashType) const {

    QByteArray res;
    const auto kFactoryKeySlot = VS_KEY_SLOT_STD_TMP_0;

    // Check input parameters
    CHECK_RET(!key.isNull(), res, "Wrong Private key for signature");

    // Set Factory private key to TMP slot
    CHECK_RET(VS_CODE_OK == m_secmoduleImpl->set_keypair(kFactoryKeySlot,
                                                         key->ecType(),
                                                         reinterpret_cast<const uint8_t*>(key->val().data()),
                                                         key->val().size(),
                                                         NULL,
                                                         0),
              res,
              "Cannot save private key to TMP slot for data signature");

    // Create HASH for data to be signed
    int hashSzMax = vs_secmodule_get_hash_len(hashType);
    uint8_t hash[hashSzMax];
    uint16_t hashResSz;
    CHECK_RET(VS_CODE_OK == m_secmoduleImpl->hash(hashType,
                                                  reinterpret_cast<const uint8_t*>(data.data()),
                                                  data.size(),
                                                  hash,
                                                  hashSzMax,
                                                  &hashResSz),
              res,
              "ERROR while creating hash");

    // Sign own public key by factory's private key
    int signSzMax = vs_secmodule_get_signature_len(key->ecType());
    uint8_t sign[signSzMax];
    uint16_t signResSz;

    CHECK_RET(VS_CODE_OK == m_secmoduleImpl->ecdsa_sign(kFactoryKeySlot,
                                                        hashType,
                                                        hash,
                                                        sign,
                                                        signSzMax,
                                                        &signResSz),
              res,
              "Cannot sign data");

    return QByteArray(reinterpret_cast<char*>(sign), signResSz);
}
//-----------------------------------------------------------------------------
