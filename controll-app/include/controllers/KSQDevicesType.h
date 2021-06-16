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

#ifndef YIOT_DEVICES_TYPE_H
#define YIOT_DEVICES_TYPE_H

#include <QtCore>
#include <QHash>
#include <QAbstractTableModel>

#include <virgil/iot/qt/VSQIoTKit.h>
#include <controllers/KSQDevice.h>

#include <yiot-iotkit/snap/KSQSnapUSERClient.h>

class KSQDevicesType : public QAbstractTableModel {
    Q_OBJECT

public:
    Q_PROPERTY(bool collapsed READ collapsed WRITE setCollapsed NOTIFY fireCollapsedChanged)

    enum Element { Name = Qt::UserRole, Type, Mac, Active, Device, Secure, Js, ElementMax };

    KSQDevicesType(QQmlApplicationEngine &engine, uint64_t deviceId);
    virtual ~KSQDevicesType() = default;

    QString
    name() const;

    QString
    type() const;

    QString
    image() const;

    QSharedPointer<QObject>
    qmlProcessor() const;

    Q_INVOKABLE void
    setCollapsed(bool c) {
        if (m_collapsed != c) {
            m_collapsed = c;
            emit fireCollapsedChanged(m_collapsed);
        }
    }

    Q_INVOKABLE bool
    collapsed() {
        return m_collapsed;
    }

    /**
     * QAbstractTableModel implementation
     */
    int
    rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int
    columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant
    data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray>
    roleNames() const override;

public slots:
    void
    onSetDeviceName(VSQMac mac, QString name);

    void
    onRequestSessionKey(VSQMac mac);

signals:
    void
    fireCollapsedChanged(bool);

    void
    fireAboutToActivate();

    void
    fireActivated();

    void
    fireRequiredSetup(QSharedPointer<KSQDevice> device);

private slots:
    // SNAP::INFO
    void
    onDeviceInfoUpdate(const VSQDeviceInfo &deviceInfo);

    //     SNAP::USER
    void
    onDeviceStateUpdate(const vs_mac_addr_t mac, QString data);

    void
    onPCError(const vs_mac_addr_t mac);

    // SNAP::SCRT
    void
    onSessionKeyReady(VSQMac mac, KSQSessionKey sessionKey);

    void
    onSessionKeyError(VSQMac mac);

    // UI
    void
    onInvokeCommand(QString mac, QString json);

private:
    bool m_collapsed = false;

private:
    bool m_active = false;
    uint64_t m_deviceTypeId;
    std::list<QSharedPointer<KSQDevice>> m_devices;
    QSharedPointer<QObject> m_qmlProcessor;

    std::pair<int, QSharedPointer<KSQDevice>>
    findDevice(const vs_mac_addr_t &mac);
};

#endif // YIOT_DEVICES_TYPE_H
