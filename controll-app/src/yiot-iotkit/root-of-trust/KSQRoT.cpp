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

const QString KSQRoT::kLocalID = "local_rot";

//-----------------------------------------------------------------------------
KSQRoT::KSQRoT(const QString &id, const QString &image) : QObject() {
    m_image = image;
    m_isValid = load(id);
    if (!m_isValid && id == kLocalID) {
        m_isValid = generate("Local Root of trust");
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

    qDebug() << "Root of trust: generate " << name;

    // Generate required amount of keys
    for (int i = 0; i < _keysCnt; i++) {
        auto keyPair = KSQSecModule::instance().generateKeypair(VS_KEYPAIR_EC_SECP256R1);
        if (keyPair.first.isNull() || keyPair.second.isNull()) {
            qDebug() << "Cannot generate key pair for a new root of trust";
            return false;
        }
        keyPairs[i] = keyPair;
    }

    // Store generated keys for SecBox

//
//    vs_secbox_type_t type, vs_storage_element_id_t id, const QByteArray &data
//
//    KSQSecBox::instance().save(VS_SECBOX_SIGNED_AND_ENCRYPTED, );

    // Generate TrustList

    m_name = name;
    m_id = kLocalID;

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQRoT::save() const {
    qDebug() << "Root of trust: save " << m_id;
    return false;
}

//-----------------------------------------------------------------------------
bool
KSQRoT::load(const QString &id) {
    qDebug() << "Root of trust: load " << id;
    return false;
}

//-----------------------------------------------------------------------------
