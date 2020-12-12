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

#ifndef YIOT_PROVISION_H
#define YIOT_PROVISION_H

#include <QtCore>

#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <yiot-iotkit/storages/KSQStorageFS.h>
#include <yiot-iotkit/root-of-trust/KSQRoT.h>

#include <virgil/iot/provision/provision.h>

class KSQProvision : public QObject, public VSQSingleton<KSQProvision> {
    Q_OBJECT

    friend VSQSingleton<KSQProvision>;
public:

    bool
    create(QSharedPointer<KSQRoT>);

    bool
    isValid() const {
        return m_valid;
    }

signals:

public slots:

private:
    KSQProvision();
    virtual ~KSQProvision();

    bool
    prepareOwnKeyPair();

    bool
    saveElement(vs_provision_element_id_e element, const QByteArray &data);

    bool m_valid;
    KSQStorageFS *m_storage;

    KSQPublicKey m_ownPubic;

    KSQPublicKey m_recovery1;
    KSQPublicKey m_recovery2;

    KSQPublicKey m_auth1;
    KSQPublicKey m_auth2;

    KSQPublicKey m_tl1;
    KSQPublicKey m_tl2;

    KSQPublicKey m_firmware1;
    KSQPublicKey m_firmware2;
};

#endif // YIOT_PROVISION_H
