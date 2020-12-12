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

using namespace VirgilIoTKit;

class KSQPublicKey : public QObject {
    Q_OBJECT

public:
    KSQPublicKey();
    KSQPublicKey(vs_secmodule_keypair_type_e keypair_type, const QByteArray &key);

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
        return m_keypair_type;
    }

signals:

public slots:

private:
    bool m_isValid;
    QByteArray m_key;
    vs_secmodule_keypair_type_e m_keypair_type;
};

#endif // YIOT_ROT_PUBLIC_KEY_H
