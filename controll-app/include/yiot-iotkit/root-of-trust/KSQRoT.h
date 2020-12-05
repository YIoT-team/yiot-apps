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

#ifndef YIOT_ROOT_OF_TRUST_H
#define YIOT_ROOT_OF_TRUST_H

#include <QtCore>

#include <yiot-iotkit/secmodule/KSQPublicKey.h>
#include <yiot-iotkit/root-of-trust/KSQTrustList.h>

class KSQRoTController;

class KSQRoT : public QObject {
    Q_OBJECT
    friend KSQRoTController;

public:
    KSQRoT() : QObject() {
    }

    KSQRoT(const QString &id, const QString &name, const QString &image = "");

    KSQRoT(const KSQRoT &r);

    virtual ~KSQRoT() = default;

    QString
    id() const;

    QString
    name() const;

    QString
    image() const;

    QString
    ecType() const {
        return "NIST256";
    }

    const KSQPublicKey &
    recovery1() const {
        return m_recovery1;
    }
    const KSQPublicKey &
    recovery2() const {
        return m_recovery2;
    }

    const KSQPublicKey &
    auth1() const {
        return m_auth1;
    }
    const KSQPublicKey &
    auth2() const {
        return m_auth2;
    }

    const KSQPublicKey &
    tl1() const {
        return m_tl1;
    }
    const KSQPublicKey &
    tl2() const {
        return m_tl2;
    }

    const KSQPublicKey &
    firmware1() const {
        return m_firmware1;
    }
    const KSQPublicKey &
    firmware2() const {
        return m_firmware2;
    }

    const KSQTrustList &
    trustList() const {
        return m_trustList;
    }

    bool
    isValid() const {
        return m_isValid;
    }

    static const QString kLocalID;

    QString
    generate(const QString &name);

signals:

public slots:

private:
    bool m_isValid;
    QString m_id;
    QString m_name;
    QString m_image;

    KSQPublicKey m_recovery1;
    KSQPublicKey m_recovery2;

    KSQPublicKey m_auth1;
    KSQPublicKey m_auth2;

    KSQPublicKey m_tl1;
    KSQPublicKey m_tl2;

    KSQPublicKey m_firmware1;
    KSQPublicKey m_firmware2;

    KSQTrustList m_trustList;

    bool
    save() const;

    bool
    load(const QString &id);
};

Q_DECLARE_METATYPE(KSQRoT)
Q_DECLARE_METATYPE(KSQRoT *)

#endif // YIOT_ROOT_OF_TRUST_H
