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

class KSQDeviceSetupData : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER m_name NOTIFY fireNameChanged)
    Q_PROPERTY(QString manufacturer MEMBER m_manufacturer NOTIFY fireManufacturerChanged)
    Q_PROPERTY(bool hasProvision MEMBER m_hasProvision NOTIFY fireHasProvisionChanged)
    Q_PROPERTY(bool hasOwner MEMBER m_hasOwner NOTIFY fireHasOwnerChanged)
    Q_PROPERTY(bool ownerIsYou MEMBER m_ownerIsYou NOTIFY fireOwnerIsYouChanged)
    Q_PROPERTY(bool needCreds MEMBER m_needCreds NOTIFY fireNeedCredsChanged)

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

private:
    QString m_name;
    QString m_manufacturer;

    bool m_hasProvision;
    bool m_hasOwner;
    bool m_ownerIsYou;
    bool m_needCreds;
};

#endif // _YIOT_QT_DEVICE_SETUP_DATA_H_
