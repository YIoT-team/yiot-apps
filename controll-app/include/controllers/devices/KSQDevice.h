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

#ifndef YIOT_DEVICE_H
#define YIOT_DEVICE_H

#include <QtCore>

#include <virgil/iot/qt/helpers/VSQMac.h>

#include <yiot-iotkit/secmodule/KSQSessionKey.h>

class KSQDevice : public QObject {
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
    Q_PROPERTY(QString commandState READ commandState WRITE setCommandState NOTIFY fireCommandStateChanged)
    Q_PROPERTY(bool hasOwner READ hasOwner NOTIFY fireHasOwnerChanged)
    Q_PROPERTY(bool hasProvision READ hasProvision NOTIFY fireHasProvisionChanged)
    Q_PROPERTY(bool hasSessionKey READ hasSessionKey NOTIFY fireHasSessionKeyChanged)
    Q_PROPERTY(bool active READ active NOTIFY fireActiveChanged)
    Q_PROPERTY(QObject *js READ js NOTIFY fireJsChanged)

public:
    KSQDevice();
    KSQDevice(QSharedPointer<QObject> js, VSQMac mac, QString name, QString img = "");
    KSQDevice(const KSQDevice &d);

    Q_INVOKABLE QString
    deviceType() const;

    Q_INVOKABLE QString
    macAddr() const;

    Q_INVOKABLE void
    invokeCommand(QString json);

    VSQMac
    qMacAddr() const;

    Q_INVOKABLE void
    setName(QString name);

    Q_INVOKABLE void
    setNameToHardware(QString name);

    Q_INVOKABLE void
    waitReboot();

    void
    setRoles(QString val);

    void
    setManufacture(QString val);

    void
    setDeviceID(QString val);

    void
    setFwVersion(QString val);

    void
    setTlVersion(QString val);

    void
    setSentBytes(QString val);

    void
    setReceivedBytes(QString val);

    void
    setHasOwner(bool hasOwner);

    void
    setHasProvision(bool hasProvision);

    void
    setSessionKey(const KSQSessionKey &key);

    void
    commandStart();

    void
    commandError();

    void
    commandDone();

    QString
    name() const;
    QString
    roles() const;
    QString
    manufacture() const;
    QString
    deviceID() const;
    QString
    fwVersion() const;
    QString
    tlVersion() const;
    QString
    sentBytes() const;
    QString
    receivedBytes() const;
    QString
    commandState() const;
    bool
    hasOwner() const;
    bool
    hasProvision() const;
    bool
    hasSessionKey() const;
    bool
    active() const;
    bool
    isWaitingReboot() const;

    bool
    operator<(const KSQDevice &rhs) const;

    bool
    isUpdatedName();

    void
    dropSession();

    static const QString kCmdStateConnect;
    static const QString kCmdStateSend;
    static const QString kCmdStateReceive;
    static const QString kCmdStateDisconnect;
    static const QString kCmdStateDone;
    static const QString kCmdStateError;

protected:
    void
    setCommandState(QString val);

    void
    _setRecivedName(QString name);

    void
    _setRecivedActivity(bool active);

    QString
    _deviceType() const {
        return "";
    }

    QObject *
    js() const;

signals:
    void
    fireNameChanged();
    void
    fireSendNameUpdate();
    void
    fireActiveChanged();
    void
    fireJsChanged();

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
    fireHasOwnerChanged();
    void
    fireHasSessionKeyChanged();
    void
    fireHasProvisionChanged();

    void
    fireDeviceStateChanged();
    void
    fireCommandStateChanged();

    void
    fireSetNameToHardware(VSQMac mac, QString name);

    void
    fireRequestSessionKey(VSQMac mac);

    void
    fireInvokeCommand(QString mac, QString json);

    void
    fireStateImageChanged();

    void
    fireRebooted();

    void
    fireSessionKeyReceived(KSQDevice *);

private:
    bool m_active;
    QDateTime m_lastUpdate;
    QString m_image;
    QString m_name;
    bool m_nameUpdated;
    VSQMac m_mac;

    QString m_roles;
    QString m_manufacture;
    QString m_deviceID;
    QString m_fwVersion;
    QString m_tlVer;
    QString m_sentBytes;
    QString m_receivedBytes;
    QString m_commandState;

    bool m_hasProvision;
    bool m_hasOwner;

    bool m_waitReboot;

    QSharedPointer<QObject> m_js;

    // TODO: Check if i need a separate class for Session
    QTimer m_sessionTimer;
    KSQSessionKey m_sessionKey;
    static const int kSessionCheckPeriodMs = 2000;

    void
    startSessionConnection();

private slots:
    void
    onSessionTimer();
};

#endif // YIOT_DEVICE_H
