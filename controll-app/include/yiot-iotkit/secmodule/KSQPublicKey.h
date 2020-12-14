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

#ifndef YIOT_ROT_PUBLIC_KEY_H
#define YIOT_ROT_PUBLIC_KEY_H

#include <QtCore>

#include <virgil/iot/secmodule/secmodule.h>
#include <virgil/iot/provision/provision-structs.h>

using namespace VirgilIoTKit;

class KSQPublicKey : public QObject {
    Q_OBJECT

public:
    KSQPublicKey();
    KSQPublicKey(vs_secmodule_keypair_type_e keypairType,
                 const QByteArray &key,
                 vs_key_type_e provisionType = VS_KEY_UNSUPPORTED,
                 const QByteArray &signature = QByteArray());

    KSQPublicKey &
    operator=(const KSQPublicKey &k);

    virtual ~KSQPublicKey() = default;

    QString
    description() const;

    const QByteArray &
    val() const {
        return m_key;
    }

    const vs_secmodule_keypair_type_e
    ecType() const {
        return m_ecType;
    }

    const vs_key_type_e
    provisionType() const {
        return m_provisionType;
    }

    KSQPublicKey &
    setProvisionType(vs_key_type_e provType) {
        m_provisionType = provType;
        return *this;
    }

    QByteArray
    datedKey() const;

    QByteArray
    signature() const;

    KSQPublicKey &
    setSignature(const QByteArray& signature);

signals:

public slots:

private:
    bool m_valid;
    vs_key_type_e m_provisionType;
    QByteArray m_key;
    vs_secmodule_keypair_type_e m_ecType;
    QByteArray m_signature;
};

#endif // YIOT_ROT_PUBLIC_KEY_H
