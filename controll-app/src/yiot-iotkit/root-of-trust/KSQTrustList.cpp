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

#include <yiot-iotkit/root-of-trust/KSQRoT.h>
#include <yiot-iotkit/root-of-trust/KSQTrustList.h>

#include <virgil/iot/trust_list/trust_list.h>
#include <virgil/iot/secmodule/secmodule-helpers.h>

//-----------------------------------------------------------------------------
KSQTrustList::KSQTrustList(const QString &id): QObject() {
    m_valid = false;
    if (id.isEmpty()) {
        return;
    }
    m_valid = load(id);
}

//-----------------------------------------------------------------------------
KSQTrustList::KSQTrustList(const KSQTrustList &tl) {
    m_valid = false;
}

//-----------------------------------------------------------------------------
QString
KSQTrustList::version() const {
    return "0.1.0.3578";
}

//-----------------------------------------------------------------------------
int
KSQTrustList::keysCount() const {
    return 11;
}

//-----------------------------------------------------------------------------
bool
KSQTrustList::create(const QString &id, const KSQRoT &rot) {
    // Check input parameters
    CHECK_NOT_ZERO_RET(!rot.factory().second.isNull(), false);
    CHECK_NOT_ZERO_RET(!rot.auth1Full().first.isNull(), false);
    CHECK_NOT_ZERO_RET(!rot.auth1Full().second.isNull(), false);
    CHECK_NOT_ZERO_RET(!rot.tl1Full().first.isNull(), false);
    CHECK_NOT_ZERO_RET(!rot.tl1Full().second.isNull(), false);

    // Calculated requires sizes
    const size_t headerSz = sizeof(vs_tl_header_t);
    const size_t bodySz = sizeof(vs_pubkey_dated_t)
                          + vs_secmodule_get_pubkey_len(rot.factory().second->ecType());
    const size_t authSignSz = + sizeof(vs_sign_t)
                              + vs_secmodule_get_signature_len(rot.auth1Full().first->ecType())
                              + vs_secmodule_get_pubkey_len(rot.auth1Full().first->ecType());
    const size_t tlSignSz = + sizeof(vs_sign_t)
                            + vs_secmodule_get_signature_len(rot.tl1Full().first->ecType())
                            + vs_secmodule_get_pubkey_len(rot.tl1Full().first->ecType());
    const size_t footerSz = sizeof(vs_tl_footer_t)
                            + authSignSz
                            + tlSignSz;
    const size_t requiredSz = headerSz + bodySz + footerSz;
    const size_t signSz = headerSz + bodySz + sizeof(vs_tl_footer_t);

    // Prepare buffers
    m_tl.resize(requiredSz);

    vs_tl_header_t *tlHeader = reinterpret_cast<vs_tl_header_t *>(m_tl.data());
    vs_pubkey_dated_t *factoryKey = reinterpret_cast<vs_pubkey_dated_t *>(m_tl.data() + headerSz);
    vs_tl_footer_t *tlFooter = reinterpret_cast<vs_tl_footer_t *>(m_tl.data() + headerSz + bodySz);

    // -----------
    //  Header
    // -----------
    tlHeader->tl_size = requiredSz;

    // Use only factory key
    tlHeader->pub_keys_count = 1;

    // Sign by Auth and TL keys
    tlHeader->signatures_count = 2;

    // Version
    tlHeader->version.major = 0;
    tlHeader->version.minor = 1;
    tlHeader->version.patch = 0;
    tlHeader->version.build = 0;
    tlHeader->version.timestamp = QDateTime::currentDateTime().toSecsSinceEpoch() - VS_START_EPOCH;

    // -----------
    //  Body
    // -----------
    factoryKey->start_date = tlHeader->version.timestamp;
    factoryKey->expire_date = 0;
    factoryKey->pubkey.ec_type = rot.factory().second->ecType();
    factoryKey->pubkey.key_type = VS_KEY_FACTORY;
    factoryKey->pubkey.meta_data_sz = 0;
    memcpy(factoryKey->pubkey.meta_and_pubkey,
           rot.factory().second->val().data(),
           rot.factory().second->val().size());

    // -----------
    //  Footer
    // -----------
    tlFooter->tl_type = TL_STORAGE_TYPE_STATIC;
    vs_sign_t *signAuth = reinterpret_cast<vs_sign_t *>(tlFooter->signatures);
    vs_sign_t *signTl = reinterpret_cast<vs_sign_t *>(
            reinterpret_cast<uint8_t *>(signAuth) + authSignSz
            );

    // Data to be signed
    const auto dataToSign = QByteArray::fromRawData(m_tl.data(), signSz);

    // Signature by Auth key
    signAuth->ec_type = rot.auth1Full().first->ecType();
    signAuth->hash_type = VS_HASH_SHA_256;
    signAuth->signer_type = VS_KEY_AUTH;
    auto signAuthData = KSQSecModule::instance().sign(dataToSign, rot.auth1Full().first);
    CHECK_RET(signAuthData.size(), false, "Cannot sign TrustList by Auth key");
    memcpy(signAuth->raw_sign_pubkey, signAuthData.data(), signAuthData.size());
    uint8_t *pubBuf = signAuth->raw_sign_pubkey + signAuthData.size();
    memcpy(pubBuf, rot.auth1Full().second->val().data(), rot.auth1Full().second->val().size());

    // Signature by TL key
    signTl->ec_type = rot.tl1Full().first->ecType();
    signTl->hash_type = VS_HASH_SHA_256;
    signTl->signer_type = VS_KEY_TRUSTLIST;
    auto signTlData = KSQSecModule::instance().sign(dataToSign, rot.tl1Full().first);
    CHECK_RET(signTlData.size(), false, "Cannot sign TrustList by TL key");
    memcpy(signTl->raw_sign_pubkey, signTlData.data(), signTlData.size());
    uint8_t *pubBufTl = signTl->raw_sign_pubkey + signTlData.size();
    memcpy(pubBufTl, rot.tl1Full().second->val().data(), rot.tl1Full().second->val().size());

    m_id = id;

    return save();
}

//-----------------------------------------------------------------------------
bool
KSQTrustList::fillStorageId(vs_storage_element_id_t id) const {
    static const QString suffix = "_trust_list";

    memset(id, 0, VS_STORAGE_ELEMENT_ID_MAX);

    auto name = m_id + suffix;
    if (name.length() >= VS_STORAGE_ELEMENT_ID_MAX) {
        VS_LOG_ERROR("Cannot create TrustList name. TL id too long.");
        return false;
    }

    strcpy(reinterpret_cast<char*>(id), name.toStdString().c_str());

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQTrustList::save() const {
    vs_storage_element_id_t id;
    CHECK_NOT_ZERO_RET(fillStorageId(id), false);

    CHECK_NOT_ZERO_RET(KSQSecBox::instance().save(
            VS_SECBOX_SIGNED_AND_ENCRYPTED,
            id,
            m_tl), false);

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQTrustList::load(const QString &id) {
    m_id = id;
    m_valid = false;
    vs_storage_element_id_t storageId;
    CHECK_NOT_ZERO_RET(fillStorageId(storageId), false);
    CHECK_NOT_ZERO_RET(KSQSecBox::instance().load(storageId, m_tl), false);
    m_valid = true;
    return m_valid;
}

//-----------------------------------------------------------------------------
