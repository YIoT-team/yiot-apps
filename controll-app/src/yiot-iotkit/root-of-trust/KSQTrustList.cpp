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

#include <endian-config.h>

//-----------------------------------------------------------------------------
KSQTrustList::KSQTrustList(const QString &id) : QObject() {
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
    if (!m_valid || m_tl.size() <= minSz()) {
        return 0;
    }

    auto header = reinterpret_cast<const vs_tl_header_t *>(m_tl.data());
    return VS_IOT_NTOHS(header->pub_keys_count);
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
    const size_t bodySz = sizeof(vs_pubkey_dated_t) + vs_secmodule_get_pubkey_len(rot.factory().second->ecType());
    const size_t authSignSz = +sizeof(vs_sign_t) + vs_secmodule_get_signature_len(rot.auth1Full().first->ecType()) +
                              vs_secmodule_get_pubkey_len(rot.auth1Full().first->ecType());
    const size_t tlSignSz = +sizeof(vs_sign_t) + vs_secmodule_get_signature_len(rot.tl1Full().first->ecType()) +
                            vs_secmodule_get_pubkey_len(rot.tl1Full().first->ecType());
    const size_t footerSz = sizeof(vs_tl_footer_t) + authSignSz + tlSignSz;
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

    vs_tl_header_to_net(tlHeader, tlHeader);

    // -----------
    //  Body
    // -----------
    factoryKey->start_date = tlHeader->version.timestamp;
    factoryKey->expire_date = 0;
    factoryKey->pubkey.ec_type = rot.factory().second->ecType();
    factoryKey->pubkey.key_type = VS_KEY_FACTORY;
    factoryKey->pubkey.meta_data_sz = 0;
    auto rawKey = rot.factory().second->val();
    memcpy(factoryKey->pubkey.meta_and_pubkey, rawKey, rawKey.size());

    // -----------
    //  Footer
    // -----------
    tlFooter->tl_type = TL_STORAGE_TYPE_STATIC;

    // Data to be signed
    const auto dataToSign = QByteArray::fromRawData(m_tl.data(), signSz);

    // Sign data
    auto authSign = KSQSecModule::instance().sign(dataToSign, rot.auth1Full());
    auto tlSign = KSQSecModule::instance().sign(dataToSign, rot.tl1Full());

    // Set signatures to TrustList
    auto allSignatures = authSign + tlSign;
    memcpy(tlFooter->signatures, allSignatures.data(), allSignatures.size());

    m_id = id;
    m_valid = save();

    return m_valid;
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

    strcpy(reinterpret_cast<char *>(id), name.toStdString().c_str());

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQTrustList::save() const {
    vs_storage_element_id_t id;
    CHECK_NOT_ZERO_RET(fillStorageId(id), false);

    CHECK_NOT_ZERO_RET(KSQSecBox::instance().save(VS_SECBOX_SIGNED_AND_ENCRYPTED, id, m_tl), false);

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
bool
KSQTrustList::set(const QByteArray &val) {
    m_tl = val;
    m_valid = save();
    return m_valid;
}

//-----------------------------------------------------------------------------
QByteArray
KSQTrustList::header() const {
    if (!m_valid || m_tl.size() <= minSz()) {
        return QByteArray();
    }

    return m_tl.left(sizeof(vs_tl_header_t));
}

//-----------------------------------------------------------------------------
QByteArray
KSQTrustList::key(size_t num) const {
    if (!m_valid || m_tl.size() <= minSz() || static_cast<int>(num) >= keysCount()) {
        return QByteArray();
    }

    auto keyData = reinterpret_cast<const uint8_t *>(m_tl.data() + sizeof(vs_tl_header_t));
    const vs_pubkey_dated_t *key;
    size_t keySz;
    int i = -1;
    do {
        ++i;
        key = reinterpret_cast<const vs_pubkey_dated_t *>(keyData);
        auto ecType = static_cast<vs_secmodule_keypair_type_e>(key->pubkey.ec_type);
        keySz = sizeof(vs_pubkey_dated_t) + VS_IOT_NTOHS(key->pubkey.meta_data_sz) +
                vs_secmodule_get_pubkey_len(ecType);
        keyData += keySz;
    } while (i < static_cast<int>(num));

    return QByteArray(reinterpret_cast<const char *>(key), keySz);
}

//-----------------------------------------------------------------------------
QByteArray
KSQTrustList::footer() const {
    if (!m_valid || m_tl.size() <= minSz()) {
        return QByteArray();
    }

    auto header = reinterpret_cast<const vs_tl_header_t *>(m_tl.data());
    auto keysNum = VS_IOT_NTOHS(header->pub_keys_count);
    auto signaturesNum = header->signatures_count;

    auto keyData = reinterpret_cast<const uint8_t *>(m_tl.data() + sizeof(vs_tl_header_t));
    for (int i = 0; i < keysNum; i++) {
        auto key = reinterpret_cast<const vs_pubkey_dated_t *>(keyData);
        auto ecType = static_cast<vs_secmodule_keypair_type_e>(key->pubkey.ec_type);
        auto keySz = sizeof(vs_pubkey_dated_t) + VS_IOT_NTOHS(key->pubkey.meta_data_sz) +
                     vs_secmodule_get_pubkey_len(ecType);
        keyData += keySz;
    }

    auto footerSz = sizeof(vs_tl_footer_t);
    auto footer = reinterpret_cast<const vs_tl_footer_t *>(keyData);
    auto signData = footer->signatures;

    for (int i = 0; i < signaturesNum; i++) {
        auto sign = reinterpret_cast<const vs_sign_t *>(signData);
        auto ecType = static_cast<vs_secmodule_keypair_type_e>(sign->ec_type);
        auto signSz = sizeof(vs_sign_t) + vs_secmodule_get_signature_len(ecType) + vs_secmodule_get_pubkey_len(ecType);
        signData += signSz;
        footerSz += signSz;
    }

    return QByteArray(reinterpret_cast<const char *>(footer), footerSz);
}

//-----------------------------------------------------------------------------
int
KSQTrustList::minSz() const {
    return sizeof(vs_tl_header_t) + sizeof(vs_tl_footer_t) + sizeof(vs_pubkey_dated_t);
}

//-----------------------------------------------------------------------------
