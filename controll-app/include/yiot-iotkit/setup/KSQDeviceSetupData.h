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

#ifndef _YIOT_QT_DEVICE_SETUP_DATA_H_
#define _YIOT_QT_DEVICE_SETUP_DATA_H_

#include <QtCore>

#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/qt/VSQIoTKit.h>

#include <yiot-iotkit/secmodule/KSQPublicKey.h>

using namespace VirgilIoTKit;

class KSQDeviceSetupData : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY fireNameChanged)
    Q_PROPERTY(QString manufacturer READ manufacturer NOTIFY fireManufacturerChanged)
    Q_PROPERTY(bool needCreds READ needCreds NOTIFY fireNeedCredsChanged)
    Q_PROPERTY(bool hasProvision MEMBER m_hasProvision NOTIFY fireHasProvisionChanged)
    Q_PROPERTY(bool hasOwner MEMBER m_hasOwner NOTIFY fireHasOwnerChanged)
    Q_PROPERTY(bool ownerIsYou MEMBER m_ownerIsYou NOTIFY fireOwnerIsYouChanged)
    Q_PROPERTY(QString publicKeyInfo READ publicKeyInfo NOTIFY firePublicKeyChanged)

public:
    KSQDeviceSetupData();
    KSQDeviceSetupData(KSQDeviceSetupData const &d);

    virtual ~KSQDeviceSetupData() = default;

    KSQDeviceSetupData &
    operator=(KSQDeviceSetupData const &d);

    bool
    operator==(const KSQDeviceSetupData &d) const;

    bool
    operator!=(const KSQDeviceSetupData &d) const;

    void
    setHasProvision(bool hasProvision);

    void
    setHasOwner(bool hasOwner);

    void
    setOwnerIsYou(bool ownerIsYou);

    void
    setPublicKey(const KSQPublicKey &publicKey);

    void
    setDeviceInfo(const VSQDeviceInfo &deviceInfo);

    QString
    name() const;

    QString
    manufacturer() const;

    bool
    needCreds() const;

signals:
    void
    fireNameChanged();

    void
    fireManufacturerChanged();

    void
    fireHasProvisionChanged();

    void
    fireHasOwnerChanged();

    void
    fireOwnerIsYouChanged();

    void
    fireNeedCredsChanged();

    void
    firePublicKeyChanged();

private:
    VSQDeviceInfo m_deviceInfo;

    bool m_hasProvision;
    bool m_hasOwner;
    bool m_ownerIsYou;
    KSQPublicKey m_publicKey;

    QString
    publicKeyInfo() const;
};

#endif // _YIOT_QT_DEVICE_SETUP_DATA_H_
