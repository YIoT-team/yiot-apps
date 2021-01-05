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

#include <QtCore>

#include <yiot-iotkit/secmodule/KSQPublicKey.h>
#include <virgil/iot/secmodule/secmodule-helpers.h>

#include <endian-config.h>

//-----------------------------------------------------------------------------
KSQPublicKey::KSQPublicKey() {
    m_valid = false;
}

//-----------------------------------------------------------------------------
KSQPublicKey::KSQPublicKey(vs_secmodule_keypair_type_e keypairType,
                           const QByteArray &key,
                           vs_key_type_e provisionType,
                           const QByteArray &signature,
                           QDateTime startDate,
                           QDateTime expireDate) {
    m_key = key;
    m_signature = signature;
    m_ecType = keypairType;
    m_provisionType = provisionType;
    m_startDate = startDate;
    m_expireDate = expireDate;
    m_valid = !m_key.isEmpty();

    registerType();
}

//-----------------------------------------------------------------------------
KSQPublicKey::KSQPublicKey(const KSQPublicKey &key) {
    if (this == &key) {
        return;
    }

    m_key = key.m_key;
    m_signature = key.m_signature;
    m_ecType = key.m_ecType;
    m_provisionType = key.m_provisionType;
    m_startDate = key.m_startDate;
    m_expireDate = key.m_expireDate;
    m_valid = key.m_valid;

    registerType();
}

//-----------------------------------------------------------------------------
KSQPublicKey::KSQPublicKey(const vs_pubkey_dated_t &key) {
    m_ecType = static_cast<vs_secmodule_keypair_type_e>(key.pubkey.ec_type);
    m_key = QByteArray(reinterpret_cast<const char *>(&key.pubkey.meta_and_pubkey[key.pubkey.meta_data_sz]),
                       vs_secmodule_get_pubkey_len(m_ecType));
    m_signature.clear();
    m_provisionType = static_cast<vs_key_type_e>(key.pubkey.key_type);
    m_startDate = QDateTime::fromSecsSinceEpoch(VS_START_EPOCH + key.start_date);
    m_expireDate = QDateTime::fromSecsSinceEpoch(VS_START_EPOCH + key.expire_date);
    m_valid = !m_key.isEmpty();
    registerType();
}

//-----------------------------------------------------------------------------
KSQPublicKey::KSQPublicKey(const vs_pubkey_t &key) {
    m_ecType = static_cast<vs_secmodule_keypair_type_e>(key.ec_type);
    m_key = QByteArray(reinterpret_cast<const char *>(&key.meta_and_pubkey[key.meta_data_sz]),
                       vs_secmodule_get_pubkey_len(m_ecType));
    m_signature.clear();
    m_provisionType = static_cast<vs_key_type_e>(key.key_type);
    m_startDate = QDateTime::fromSecsSinceEpoch(VS_START_EPOCH);
    m_expireDate = QDateTime::fromSecsSinceEpoch(VS_START_EPOCH);
    m_valid = !m_key.isEmpty();
    registerType();
}

//-----------------------------------------------------------------------------
KSQPublicKey &
KSQPublicKey::operator=(const KSQPublicKey &k) {
    if (this == &k) {
        return *this;
    }

    m_ecType = k.m_ecType;
    m_signature = k.m_signature;
    m_key = k.m_key;
    m_provisionType = k.m_provisionType;
    m_valid = k.m_valid;
    m_startDate = k.m_startDate;
    m_expireDate = k.m_expireDate;

    return *this;
}

//-----------------------------------------------------------------------------
QString
KSQPublicKey::description() const {
    return m_key.toBase64();
}

//-----------------------------------------------------------------------------
bool
KSQPublicKey::isValid() const {
    return m_valid;
}

//-----------------------------------------------------------------------------
const QByteArray &
KSQPublicKey::val() const {
    return m_key;
}

//-----------------------------------------------------------------------------
const vs_secmodule_keypair_type_e
KSQPublicKey::ecType() const {
    return m_ecType;
}

//-----------------------------------------------------------------------------
const vs_key_type_e
KSQPublicKey::provisionType() const {
    return m_provisionType;
}

//-----------------------------------------------------------------------------
KSQPublicKey &
KSQPublicKey::setProvisionType(vs_key_type_e provType) {
    m_provisionType = provType;
    return *this;
}

//-----------------------------------------------------------------------------
QByteArray
KSQPublicKey::datedKey() const {
    // Calculate size of result
    size_t pubKeySz = sizeof(vs_pubkey_dated_t) + vs_secmodule_get_pubkey_len(m_ecType);

    // Create result buffer
    QByteArray res;
    res.resize(pubKeySz);
    uint8_t *publicKeyBuf = reinterpret_cast<uint8_t *>(res.data());
    vs_pubkey_dated_t *pubkeyDated = reinterpret_cast<vs_pubkey_dated_t *>(publicKeyBuf);

    // Fill data
#if 0
    pubkeyDated->start_date = VS_IOT_HTONL(m_startDate.toSecsSinceEpoch() - VS_START_EPOCH);
    pubkeyDated->expire_date = VS_IOT_HTONL(m_expireDate.toSecsSinceEpoch() - VS_START_EPOCH);
#else
    pubkeyDated->start_date = 0;
    pubkeyDated->expire_date = 0;
#endif
    pubkeyDated->pubkey.key_type = m_provisionType;
    pubkeyDated->pubkey.ec_type = m_ecType;
    pubkeyDated->pubkey.meta_data_sz = 0;

    // Fill key data
    memcpy(pubkeyDated->pubkey.meta_and_pubkey + pubkeyDated->pubkey.meta_data_sz, m_key.data(), m_key.size());

    return res;
}

//-----------------------------------------------------------------------------
QByteArray
KSQPublicKey::signature() const {
    return m_signature;
}

//-----------------------------------------------------------------------------
QByteArray
KSQPublicKey::signedDatedKey() const {
    return datedKey() + signature();
}

//-----------------------------------------------------------------------------
KSQPublicKey &
KSQPublicKey::setSignature(const QByteArray &signature) {
    m_signature = signature;
    return *this;
}

//-----------------------------------------------------------------------------
QDateTime
KSQPublicKey::startDate() const {
    return m_startDate;
}

//-----------------------------------------------------------------------------
QDateTime
KSQPublicKey::expireDate() const {
    return m_expireDate;
}

//-----------------------------------------------------------------------------
void
KSQPublicKey::registerType() {
    static bool registered = false;

    if (registered) {
        return;
    }
    qRegisterMetaType<KSQPublicKey>("KSQPublicKey");
    registered = true;
}

//-----------------------------------------------------------------------------
