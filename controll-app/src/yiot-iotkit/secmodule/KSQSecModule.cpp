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
#include <virgil/iot/vs-soft-secmodule/vs-soft-secmodule.h>
#include <virgil/iot/secmodule/secmodule-helpers.h>

#include <yiot-iotkit/storages/KSQStorageKeychain.h>
#include <yiot-iotkit/storages/KSQStorageFS.h>

#include <endian-config.h>

// !!! Include private headers from Software security module !!!
using namespace VirgilIoTKit;
extern "C" {
#include <private/vs-soft-secmodule-internal.h>
}


//-----------------------------------------------------------------------------
KSQSecModule::KSQSecModule() {
#if 0
    auto storage = KSQStorageKeychain::instance().storageImpl();
#else
    auto base = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation).first();
    static KSQStorageFS *storageFS = nullptr;
    if (!storageFS) {
        storageFS = new KSQStorageFS(base + QDir::separator() + "hsm");
    }
    auto storage = storageFS->storageImpl();
#endif
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

    //    if (!vs_converters_pubkey_to_raw(
    //                keypair_type, public_key, public_key_sz, public_key, sizeof(public_key), &public_key_sz)) {
    //        qDebug() << "Unable to convert a public key to raw";
    //        return std::make_pair(nullptr, nullptr);
    //    }

    auto pubkey = QSharedPointer<KSQPublicKey>::create(keypair_type,
                                                       QByteArray(reinterpret_cast<char *>(public_key), public_key_sz));
    auto privkey = QSharedPointer<KSQPrivateKey>::create(
            keypair_type, QByteArray(reinterpret_cast<char *>(private_key), private_key_sz));

    return std::make_pair(privkey, pubkey);
}

//-----------------------------------------------------------------------------
QByteArray
KSQSecModule::signRaw(const QByteArray &data,
                      QSharedPointer<KSQPrivateKey> key,
                      vs_secmodule_hash_type_e hashType) const {

    QByteArray res;
    const auto kFactoryKeySlot = VS_KEY_SLOT_STD_TMP_0;

    // Check input parameters
    CHECK_RET(!key.isNull(), res, "Wrong Private key for signature");

    // Set Factory private key to TMP slot
    CHECK_RET(VS_CODE_OK == m_secmoduleImpl->set_keypair(kFactoryKeySlot,
                                                         key->ecType(),
                                                         reinterpret_cast<const uint8_t *>(key->val().data()),
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
                                                  reinterpret_cast<const uint8_t *>(data.data()),
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

    CHECK_RET(VS_CODE_OK == m_secmoduleImpl->ecdsa_sign(kFactoryKeySlot, hashType, hash, sign, signSzMax, &signResSz),
              res,
              "Cannot sign data");

    return QByteArray(reinterpret_cast<char *>(sign), signResSz);
}

//-----------------------------------------------------------------------------
QByteArray
KSQSecModule::sign(const QByteArray &data, const KSQKeyPair &signerKeyPair, vs_secmodule_hash_type_e hashType) const {
    QByteArray res;

    // Check input parameters
    CHECK_NOT_ZERO_RET(!signerKeyPair.first.isNull(), res);
    CHECK_NOT_ZERO_RET(!signerKeyPair.second.isNull(), res);

    // Calculate required size of buffer
    size_t signSz = sizeof(vs_sign_t) + vs_secmodule_get_signature_len(signerKeyPair.first->ecType()) +
                    vs_secmodule_get_pubkey_len(signerKeyPair.first->ecType());

    // Allocate buffer
    res.resize(signSz);

    // Fill signature data
    vs_sign_t *sign = reinterpret_cast<vs_sign_t *>(res.data());
    sign->ec_type = signerKeyPair.first->ecType();
    sign->hash_type = hashType;
    sign->signer_type = signerKeyPair.second->provisionType();

    // Sign data
    auto signRawData = KSQSecModule::instance().signRaw(data, signerKeyPair.first);
    CHECK_RET(signRawData.size(), QByteArray(), "");

    // Copy signature
    memcpy(sign->raw_sign_pubkey, signRawData.data(), signRawData.size());

    // Set signer's public key
    uint8_t *pubBuf = sign->raw_sign_pubkey + signRawData.size();
    QByteArray baDatedKey(signerKeyPair.second->datedKey());
    auto datedKey = reinterpret_cast<const vs_pubkey_dated_t *>(baDatedKey.data());
    memcpy(pubBuf,
           datedKey->pubkey.meta_and_pubkey + VS_IOT_NTOHS(datedKey->pubkey.meta_data_sz),
           vs_secmodule_get_pubkey_len(static_cast<vs_secmodule_keypair_type_e>(datedKey->pubkey.ec_type)));

    return res;
}

//-----------------------------------------------------------------------------
