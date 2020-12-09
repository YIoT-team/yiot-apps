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
#include <yiot-iotkit/secmodule/KSQKeyPair.h>
#include <yiot-iotkit/root-of-trust/KSQTrustList.h>

#include <virgil/iot/storage_hal/storage_hal.h>

class KSQRoTController;

class KSQRoT : public QObject {
    Q_OBJECT
    friend KSQRoTController;

public:
    KSQRoT() : QObject() {
    }

    KSQRoT(const QString &id, const QString &image = "");

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
        return *m_recovery1.second.get();
    }
    const KSQPublicKey &
    recovery2() const {
        return *m_recovery2.second.get();
    }

    const KSQPublicKey &
    auth1() const {
        return *m_auth1.second.get();
    }
    const KSQPublicKey &
    auth2() const {
        return *m_auth2.second.get();
    }

    const KSQPublicKey &
    tl1() const {
        return *m_tl1.second.get();
    }
    const KSQPublicKey &
    tl2() const {
        return *m_tl2.second.get();
    }

    const KSQPublicKey &
    firmware1() const {
        return *m_firmware1.second.get();
    }
    const KSQPublicKey &
    firmware2() const {
        return *m_firmware2.second.get();
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

    bool
    generate(const QString &name);

signals:

public slots:

private:
    static const QString kNamePrivate;
    static const QString kNamePublic;

    static const QString kNameRecovery1;
    static const QString kNameRecovery2;

    static const QString kNameAuth1;
    static const QString kNameAuth2;

    static const QString kNameTl1;
    static const QString kNameTl2;

    static const QString kNameFirmware1;
    static const QString kNameFirmware2;

    static const QString kNameFactory;

    static const QString kNameTrustList;

    bool m_isValid;
    QString m_id;
    QString m_name;
    QString m_image;

    KSQKeyPair m_recovery1;
    KSQKeyPair m_recovery2;

    KSQKeyPair m_auth1;
    KSQKeyPair m_auth2;

    KSQKeyPair m_tl1;
    KSQKeyPair m_tl2;

    KSQKeyPair m_firmware1;
    KSQKeyPair m_firmware2;

    KSQKeyPair m_factory;

    KSQTrustList m_trustList;

    bool
    prepName(const QString &name, vs_storage_element_id_t id) const;

    bool
    save() const;

    bool
    saveKeyPair(const QString &name, const KSQKeyPair &keypair) const;

    bool
    load(const QString &id);
};

Q_DECLARE_METATYPE(KSQRoT)
Q_DECLARE_METATYPE(KSQRoT *)

#endif // YIOT_ROOT_OF_TRUST_H
