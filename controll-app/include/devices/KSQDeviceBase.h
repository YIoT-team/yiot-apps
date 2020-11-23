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

#ifndef YIOT_DEVICE_BASE_H
#define YIOT_DEVICE_BASE_H

#include <QtCore>

#include <virgil/iot/qt/helpers/VSQMac.h>

class KSQDeviceBase : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString deviceType READ deviceType)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY fireNameChanged)
    Q_PROPERTY(QString macAddr READ macAddr)
    Q_PROPERTY(QString roles READ roles WRITE setRoles NOTIFY fireRolesChanged)
    Q_PROPERTY(QString manufacture READ manufacture WRITE setManufacture NOTIFY fireManufactureChanged)
    Q_PROPERTY(QString deviceID READ deviceID WRITE setDeviceID NOTIFY fireDeviceIDChanged)
    Q_PROPERTY(QString fwVersion READ fwVersion WRITE setFwVersion NOTIFY fireFwVersionChanged)
    Q_PROPERTY(QString tlVersion READ tlVersion WRITE setTlVersion NOTIFY fireTlVerChanged)
    Q_PROPERTY(QString sentBytes READ sentBytes WRITE setSentBytes NOTIFY fireSentBytesChanged)
    Q_PROPERTY(QString receivedBytes READ receivedBytes WRITE setReceivedBytes NOTIFY fireReceivedBytesChanged)
    Q_PROPERTY(bool active READ active NOTIFY fireActiveChanged)

public:
    KSQDeviceBase() {
    }
    KSQDeviceBase(VSQMac mac, QString name, QString img = "") {
        m_lastUpdate = QDateTime::currentDateTime();
        m_image = img;
        m_name = name;
        m_mac = mac;
        m_active = true;
    }

    KSQDeviceBase(const KSQDeviceBase &d) {
        m_lastUpdate = d.m_lastUpdate;
        m_image = d.m_image;
        m_name = d.m_name;
        m_mac = d.m_mac;
        m_active = d.m_active;
    }

    Q_INVOKABLE QString
    deviceType() const {
        return _deviceType();
    }

    Q_INVOKABLE QString
    macAddr() const {
        return m_mac;
    }

    VSQMac
    qMacAddr() const {
        return m_mac;
    }

    Q_INVOKABLE void
    setName(QString name) {
        if (name != m_name) {
            m_name = name;
            emit fireNameChanged();
        }
        emit fireSendNameUpdate();
    }

    void
    setRoles(QString val) {
        if (val != m_roles) {
            m_roles = val;
            emit fireRolesChanged();
        }
    }

    void
    setManufacture(QString val) {
        if (val != m_manufacture) {
            m_manufacture = val;
            emit fireManufactureChanged();
        }
    }

    void
    setDeviceID(QString val) {
        if (val != m_deviceID) {
            m_deviceID = val;
            emit fireDeviceIDChanged();
        }
    }

    void
    setFwVersion(QString val) {
        if (val != m_fwVersion) {
            m_fwVersion = val;
            emit fireFwVersionChanged();
        }
    }

    void
    setTlVersion(QString val) {
        if (val != m_tlVer) {
            m_tlVer = val;
            emit fireTlVerChanged();
        }
    }

    void
    setSentBytes(QString val) {
        if (val != m_sentBytes) {
            m_sentBytes = val;
            emit fireSentBytesChanged();
        }
    }

    void
    setReceivedBytes(QString val) {
        if (val != m_receivedBytes) {
            m_receivedBytes = val;
            emit fireReceivedBytesChanged();
        }
    }

    QString
    name() const {
        return m_name;
    }
    QString
    roles() const {
        return m_roles;
    }
    QString
    manufacture() const {
        return m_manufacture;
    }
    QString
    deviceID() const {
        return m_deviceID;
    }
    QString
    fwVersion() const {
        return m_fwVersion;
    }
    QString
    tlVersion() const {
        return m_tlVer;
    }
    QString
    sentBytes() const {
        return m_sentBytes;
    }
    QString
    receivedBytes() const {
        return m_receivedBytes;
    }
    bool
    active() const {
        return m_active;
    }

    bool
    operator<(const KSQDeviceBase &rhs) const {
        return m_name < rhs.m_name;
    }

protected:
    void
    _setRecivedName(QString name) {
        if (name != m_name) {
            m_name = name;
            emit fireNameChanged();
        }
    }

    void
    _setRecivedActivity(bool active) {
        if (active != m_active) {
            m_active = active;
            emit fireActiveChanged();
        }
    }

    virtual QString
    _deviceType() const = 0;

signals:
    void
    fireNameChanged();
    void
    fireSendNameUpdate();
    void
    fireActiveChanged();

    void
    fireRolesChanged();
    void
    fireManufactureChanged();
    void
    fireDeviceIDChanged();
    void
    fireFwVersionChanged();
    void
    fireTlVerChanged();
    void
    fireSentBytesChanged();
    void
    fireReceivedBytesChanged();

    void
    fireDeviceStateChanged();

private:
    bool m_active;
    QDateTime m_lastUpdate;
    QString m_image;
    QString m_name;
    VSQMac m_mac;

    QString m_roles;
    QString m_manufacture;
    QString m_deviceID;
    QString m_fwVersion;
    QString m_tlVer;
    QString m_sentBytes;
    QString m_receivedBytes;
};

#endif // YIOT_DEVICE_BASE_H
