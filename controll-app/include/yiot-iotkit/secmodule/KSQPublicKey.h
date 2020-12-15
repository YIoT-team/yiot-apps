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
                 const QByteArray &signature = QByteArray(),
                 QDateTime startDate = QDateTime::currentDateTimeUtc(),
                 QDateTime expireDate = QDateTime::fromSecsSinceEpoch(VS_START_EPOCH));

    KSQPublicKey &
    operator=(const KSQPublicKey &k);

    virtual ~KSQPublicKey() = default;

    QString
    description() const;

    bool
    isValid() const;

    const QByteArray &
    val() const;

    const vs_secmodule_keypair_type_e
    ecType() const;

    const vs_key_type_e
    provisionType() const;

    KSQPublicKey &
    setProvisionType(vs_key_type_e provType);

    QByteArray
    datedKey() const;

    QByteArray
    signature() const;

    KSQPublicKey &
    setSignature(const QByteArray& signature);

    QDateTime
    startDate() const;

    QDateTime
    expireDate() const;

signals:

public slots:

private:
    bool m_valid;
    vs_key_type_e m_provisionType;
    QByteArray m_key;
    vs_secmodule_keypair_type_e m_ecType;
    QDateTime m_startDate;
    QDateTime m_expireDate;
    QByteArray m_signature;
};

#endif // YIOT_ROT_PUBLIC_KEY_H
