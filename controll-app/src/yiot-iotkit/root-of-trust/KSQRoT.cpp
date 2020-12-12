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

#include <QUuid>

#include <yiot-iotkit/root-of-trust/KSQRoT.h>
#include <yiot-iotkit/secmodule/KSQSecModule.h>
#include <yiot-iotkit/secbox/KSQSecBox.h>

using namespace VirgilIoTKit;

const vs_secmodule_keypair_type_e KSQRoT::kDefaultEC = VS_KEYPAIR_EC_SECP256R1;

const QString KSQRoT::kLocalID = "local_rot";

const QString KSQRoT::kNamePrivate = "_priv";
const QString KSQRoT::kNamePublic = "_pub";

const QString KSQRoT::kNameRecovery1 = "recovery_1";
const QString KSQRoT::kNameRecovery2 = "recovery_2";

const QString KSQRoT::kNameAuth1 = "auth_1";
const QString KSQRoT::kNameAuth2 = "auth_2";

const QString KSQRoT::kNameTl1 = "tl_1";
const QString KSQRoT::kNameTl2 = "tl_2";

const QString KSQRoT::kNameFirmware1 = "firmware_1";
const QString KSQRoT::kNameFirmware2 = "firmware_2";

const QString KSQRoT::kNameFactory = "factory";

const QString KSQRoT::kNameTrustList = "trust_list";

//-----------------------------------------------------------------------------
KSQRoT::KSQRoT() : QObject(), m_trustList("") {

}

//-----------------------------------------------------------------------------
KSQRoT::KSQRoT(const QString &id, const QString &image) : QObject() {
    m_image = image;
    m_valid = load(id);
    if (!m_valid && id == kLocalID) {
        m_valid = generate("Local Root of trust");
    }
}

//-----------------------------------------------------------------------------
KSQRoT::KSQRoT(const KSQRoT &r) {
}

//-----------------------------------------------------------------------------
QString
KSQRoT::id() const {
    return m_id;
}

//-----------------------------------------------------------------------------
QString
KSQRoT::name() const {
    return m_name;
}

//-----------------------------------------------------------------------------
QString
KSQRoT::image() const {
    return m_image;
}

//-----------------------------------------------------------------------------
bool
KSQRoT::generate(const QString &name) {
    static const size_t _keysCnt = 9;
    KSQKeyPair keyPairs[_keysCnt];

    VS_LOG_DEBUG("Root of trust: generate %s", name.toStdString().c_str());

    // Set information
    m_name = name;
    m_id = kLocalID;

    // Generate required amount of keys
    for (int i = 0; i < _keysCnt; i++) {
        auto keyPair = KSQSecModule::instance().generateKeypair(kDefaultEC);
        if (keyPair.first.isNull() || keyPair.second.isNull()) {
            VS_LOG_ERROR("Cannot generate key pair for a new root of trust");
            return false;
        }
        keyPairs[i] = keyPair;
    }

    m_recovery1 = keyPairs[0];
    m_recovery2 = keyPairs[1];

    m_auth1 = keyPairs[2];
    m_auth2 = keyPairs[3];

    m_tl1 = keyPairs[4];
    m_tl2 = keyPairs[5];

    m_firmware1 = keyPairs[6];
    m_firmware2 = keyPairs[7];

    m_factory = keyPairs[8];

    // Generate TrustList
    if (!m_trustList.create(m_id, *this)) {
        return false;
    }

    // Store generated keys for SecBox
    return save();
}

//-----------------------------------------------------------------------------
bool
KSQRoT::saveKeyPair(const QString &name, const KSQKeyPair &keypair) const {
    vs_storage_element_id_t id_priv;
    vs_storage_element_id_t id_pub;

    CHECK_NOT_ZERO_RET(prepName(name + kNamePrivate, id_priv), false);
    CHECK_NOT_ZERO_RET(prepName(name + kNamePublic, id_pub), false);

    CHECK_NOT_ZERO_RET(KSQSecBox::instance().save(
                               VS_SECBOX_SIGNED_AND_ENCRYPTED,
                               id_priv,
                               keypair.first.get()->val()), false);

    CHECK_NOT_ZERO_RET(KSQSecBox::instance().save(
            VS_SECBOX_SIGNED_AND_ENCRYPTED,
            id_pub,
            keypair.second.get()->val()), false);

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQRoT::loadKeyPair(const QString &name, KSQKeyPair &res) const {
    vs_storage_element_id_t id_priv;
    vs_storage_element_id_t id_pub;
    QByteArray baPriv;
    QByteArray baPub;

    res = std::make_pair(nullptr, nullptr);

    CHECK_RET(prepName(name + kNamePrivate, id_priv), false, "Cannot prepare name %s", kNamePrivate.toStdString().c_str());
    CHECK_RET(prepName(name + kNamePublic, id_pub), false, "Cannot prepare name %s", kNamePublic.toStdString().c_str());

    CHECK_RET(KSQSecBox::instance().load(id_priv, baPriv), false, "Cannot load %s", kNamePrivate.toStdString().c_str());
    CHECK_RET(KSQSecBox::instance().load(id_pub, baPub), false, "Cannot load %s", kNamePublic.toStdString().c_str());

    auto pubkey = QSharedPointer<KSQPublicKey>::create(kDefaultEC, baPub);
    auto privkey = QSharedPointer<KSQPrivateKey>::create(kDefaultEC, baPriv);

    res = std::make_pair(privkey, pubkey);
    return true;
}

//-----------------------------------------------------------------------------
bool
KSQRoT::prepName(const QString &name, vs_storage_element_id_t id) const {
    QString fullName = m_id + "__" + name;
    memset(id, 0, sizeof(vs_storage_element_id_t));

    if (fullName.size() >= sizeof(vs_storage_element_id_t)) {
        VS_LOG_ERROR("File name is greater than 32 bytes %s", name.toStdString().c_str());
        return false;
    }

    strcpy(reinterpret_cast<char *>(id), fullName.toStdString().c_str());

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQRoT::save() {
    CHECK_NOT_ZERO_RET(saveKeyPair(kNameRecovery1, m_recovery1), false);
    CHECK_NOT_ZERO_RET(saveKeyPair(kNameRecovery2, m_recovery2), false);

    CHECK_NOT_ZERO_RET(saveKeyPair(kNameAuth1, m_auth1), false);
    CHECK_NOT_ZERO_RET(saveKeyPair(kNameAuth2, m_auth2), false);

    CHECK_NOT_ZERO_RET(saveKeyPair(kNameTl1, m_tl1), false);
    CHECK_NOT_ZERO_RET(saveKeyPair(kNameTl2, m_tl2), false);

    CHECK_NOT_ZERO_RET(saveKeyPair(kNameFirmware1, m_firmware1), false);
    CHECK_NOT_ZERO_RET(saveKeyPair(kNameFirmware2, m_firmware2), false);

    CHECK_NOT_ZERO_RET(saveKeyPair(kNameFactory, m_factory), false);

    VS_LOG_DEBUG("Root of trust: saved SUCCESSFULLY %s", m_id.toStdString().c_str());

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQRoT::load(const QString &id) {
    m_id = id;

    CHECK_NOT_ZERO_RET(loadKeyPair(kNameRecovery1, m_recovery1), false);
    CHECK_NOT_ZERO_RET(loadKeyPair(kNameRecovery2, m_recovery2), false);

    CHECK_NOT_ZERO_RET(loadKeyPair(kNameAuth1, m_auth1), false);
    CHECK_NOT_ZERO_RET(loadKeyPair(kNameAuth2, m_auth2), false);

    CHECK_NOT_ZERO_RET(loadKeyPair(kNameTl1, m_tl1), false);
    CHECK_NOT_ZERO_RET(loadKeyPair(kNameTl2, m_tl2), false);

    CHECK_NOT_ZERO_RET(loadKeyPair(kNameFirmware1, m_firmware1), false);
    CHECK_NOT_ZERO_RET(loadKeyPair(kNameFirmware2, m_firmware2), false);

    CHECK_NOT_ZERO_RET(loadKeyPair(kNameFactory, m_factory), false);

    VS_LOG_DEBUG("Root of trust: loaded SUCCESSFULLY %s", m_id.toStdString().c_str());

    return m_trustList.load(id);
}

//-----------------------------------------------------------------------------
