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

#include <virgil/iot/provision/provision-structs.h>
#include <virgil/iot/secmodule/secmodule-helpers.h>

// TODO: Move it from SNAP protocol
extern "C" {
#include <virgil/iot/protocols/snap/generated/snap_cvt.h>
}

using namespace VirgilIoTKit;

const vs_secmodule_keypair_type_e KSQRoT::kDefaultEC = VS_KEYPAIR_EC_SECP256R1;
const vs_secmodule_hash_type_e KSQRoT::kDefaultHash = VS_HASH_SHA_256;

const QString KSQRoT::kLocalID = "local_rot";

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
KSQRoT::prepareProvisionKeyPair(KSQKeyPair &dst,
                                const KSQKeyPair &src,
                                vs_key_type_e provType,
                                const KSQKeyPair &signer) {
    dst = src;
    dst.second->setProvisionType(provType);

    QByteArray signature;
    if (!signer.first.isNull() && !signer.second.isNull()) {
        signature = KSQSecModule::instance().sign(dst.second->datedKey(), signer);
        if (!signature.size()) {
            VS_LOG_WARNING("Cannot sign data");
            return false;
        }
    }
    dst.second->setSignature(signature);

    return true;
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
    for (size_t i = 0; i < _keysCnt; i++) {
        auto keyPair = KSQSecModule::instance().generateKeypair(kDefaultEC);
        if (keyPair.first.isNull() || keyPair.second.isNull()) {
            VS_LOG_ERROR("Cannot generate key pair for a new root of trust");
            return false;
        }
        keyPairs[i] = keyPair;
    }

    CHECK_RET(
            prepareProvisionKeyPair(m_recovery1, keyPairs[0], VS_KEY_RECOVERY), false, "Cannot prepare Recovery Key 1");
    CHECK_RET(
            prepareProvisionKeyPair(m_recovery2, keyPairs[1], VS_KEY_RECOVERY), false, "Cannot prepare Recovery Key 2");

    CHECK_RET(prepareProvisionKeyPair(m_auth1, keyPairs[2], VS_KEY_AUTH, m_recovery1),
              false,
              "Cannot prepare Auth Key 1");
    CHECK_RET(prepareProvisionKeyPair(m_auth2, keyPairs[3], VS_KEY_AUTH, m_recovery1),
              false,
              "Cannot prepare Auth Key 2");

    CHECK_RET(prepareProvisionKeyPair(m_tl1, keyPairs[4], VS_KEY_TRUSTLIST, m_recovery1),
              false,
              "Cannot prepare TL Key 1");
    CHECK_RET(prepareProvisionKeyPair(m_tl2, keyPairs[5], VS_KEY_TRUSTLIST, m_recovery1),
              false,
              "Cannot prepare TL Key 2");

    CHECK_RET(prepareProvisionKeyPair(m_firmware1, keyPairs[6], VS_KEY_FIRMWARE, m_recovery1),
              false,
              "Cannot prepare Firmware Key 1");
    CHECK_RET(prepareProvisionKeyPair(m_firmware2, keyPairs[7], VS_KEY_FIRMWARE, m_recovery1),
              false,
              "Cannot prepare Firmware Key 2");

    CHECK_RET(prepareProvisionKeyPair(m_factory, keyPairs[8], VS_KEY_FACTORY), false, "Cannot prepare Factory Key");

    // Store generated keys for SecBox
    if (!save()) {
        VS_LOG_ERROR("Cannot save Root Of Trust");
        return false;
    }

    // Generate TrustList
    if (!m_trustList.create(m_id, *this)) {
        VS_LOG_ERROR("Cannot generate Trust List");
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
QByteArray
KSQRoT::keyPair2Data(const KSQKeyPair &keypair) const {
    QByteArray data;
    QDataStream dataStreamWrite(&data, QIODevice::WriteOnly);
    dataStreamWrite << keypair.second->ecType() << keypair.second->provisionType() << keypair.first->val()
                    << keypair.second->val() << keypair.second->startDate() << keypair.second->expireDate()
                    << keypair.second->signature();
    return data;
}

//-----------------------------------------------------------------------------
KSQKeyPair
KSQRoT::data2KeyPair(const QByteArray &data) const {
    QDataStream dataStreamRead(data);

    vs_secmodule_keypair_type_e ecType;
    vs_key_type_e provisionType;
    QByteArray privKey;
    QByteArray pubKey;
    QByteArray signature;
    QDateTime startDate;
    QDateTime expireDate;
    dataStreamRead >> ecType >> provisionType >> privKey >> pubKey >> startDate >> expireDate >> signature;

    auto privkey = QSharedPointer<KSQPrivateKey>::create(ecType, privKey);
    auto pubkey = QSharedPointer<KSQPublicKey>::create(ecType, pubKey, provisionType, signature, startDate, expireDate);
    return std::make_pair(privkey, pubkey);
}

//-----------------------------------------------------------------------------
bool
KSQRoT::saveKeyPair(const QString &name, const KSQKeyPair &keypair) const {
    vs_storage_element_id_t id;
    CHECK_NOT_ZERO_RET(prepName(name, id), false);

    CHECK_NOT_ZERO_RET(KSQSecBox::instance().save(VS_SECBOX_SIGNED_AND_ENCRYPTED, id, keyPair2Data(keypair)), false);
    return true;
}

//-----------------------------------------------------------------------------
bool
KSQRoT::loadKeyPair(const QString &name, KSQKeyPair &res) const {
    vs_storage_element_id_t id;
    CHECK_NOT_ZERO_RET(prepName(name, id), false);

    QByteArray data;
    CHECK_RET(KSQSecBox::instance().load(id, data), false, "Cannot load keypair %s", name.toStdString().c_str());

    res = data2KeyPair(data);
    return true;
}

//-----------------------------------------------------------------------------
bool
KSQRoT::prepName(const QString &name, vs_storage_element_id_t id) const {
    QString fullName = m_id + "__" + name;
    memset(id, 0, sizeof(vs_storage_element_id_t));

    if (static_cast<size_t>(fullName.size()) >= sizeof(vs_storage_element_id_t)) {
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
    m_name = id;

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
QString
KSQRoT::importData(QString fileName, QString password) {
    // Load encrypted data from file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        VS_LOG_WARNING("Cannot import RoT: file open");
        return tr("Cannot open file");
    }

    auto encData = file.readAll();
    if (encData.isEmpty()) {
        VS_LOG_WARNING("Cannot import RoT: file is empty");
        return tr("File is empty");
    }

    // Decrypt data array
    auto data = KSQSecModule::instance().decryptWithPassword(encData, password);
    if (data.isEmpty()) {
        VS_LOG_WARNING("Cannot import RoT: decryption error");
        return tr("File cannot be decrypted");
    }

    // Load elements
    QByteArray r1, r2, a1, a2, tl1, tl2, fw1, fw2, f1, trustList;
    QDataStream dataStreamRead(data);
    dataStreamRead >> r1 >> r2 >> a1 >> a2 >> tl1 >> tl2 >> fw1 >> fw2 >> f1 >> trustList;
    // Find the last ByteArray as a trust list
    while (true) {
        QByteArray tmp;
        dataStreamRead >> tmp;
        if (tmp.isEmpty()) {
            break;
        }
        trustList = tmp;
    }

    if (r1.isEmpty() || r2.isEmpty() || a1.isEmpty() || a2.isEmpty() || tl1.isEmpty() || tl2.isEmpty() ||
        fw1.isEmpty() || fw2.isEmpty() || f1.isEmpty() || trustList.isEmpty()) {
        VS_LOG_WARNING("Some data is empry");
        return tr("Wrong file format");
    }

    m_recovery1 = data2KeyPair(r1);
    m_recovery2 = data2KeyPair(r2);

    m_auth1 = data2KeyPair(a1);
    m_auth2 = data2KeyPair(a2);

    m_tl1 = data2KeyPair(tl1);
    m_tl2 = data2KeyPair(tl2);

    m_firmware1 = data2KeyPair(fw1);
    m_firmware2 = data2KeyPair(fw2);

    m_factory = data2KeyPair(f1);

    if (!m_trustList.set(trustList)) {
        VS_LOG_WARNING("Cannot import RoT: TrustList save error");
        return tr("Wrong file format");
    }

    if (!save()) {
        VS_LOG_WARNING("Cannot import RoT: save error");
        return tr("Cannot save imported data");
    }

    emit fireUpdated(*this);

    return tr("Done successfully");
}

//-----------------------------------------------------------------------------
QString
KSQRoT::exportData(QString fileName, QString password) const {
    // Collect all key pairs
    auto r1 = keyPair2Data(m_recovery1);
    auto r2 = keyPair2Data(m_recovery2);

    auto a1 = keyPair2Data(m_auth1);
    auto a2 = keyPair2Data(m_auth2);

    auto tl1 = keyPair2Data(m_tl1);
    auto tl2 = keyPair2Data(m_tl2);

    auto fw1 = keyPair2Data(m_firmware1);
    auto fw2 = keyPair2Data(m_firmware2);

    auto f1 = keyPair2Data(m_factory);

    // Create common data array
    QByteArray data;
    QDataStream dataStreamWrite(&data, QIODevice::WriteOnly);
    dataStreamWrite << r1 << r2 << a1 << a2 << tl1 << tl2 << fw1 << fw2 << f1 << m_trustList.val();

    // Encrypt data array
    auto encData = KSQSecModule::instance().encryptWithPassword(data, password);

    // Save encrypted data to file
    QSaveFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        VS_LOG_WARNING("Cannot export RoT: file open");
        return tr("Cannot create file");
    }

    if (encData.size() != file.write(encData) || !file.commit()) {
        VS_LOG_WARNING("Cannot export RoT: file save");
        return tr("Cannot write file");
    }

    return tr("Done successfully");
}

//-----------------------------------------------------------------------------
