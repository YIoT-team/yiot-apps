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

#ifndef KSQ_BLANK_DEVICES_CONTROLLER_H
#define KSQ_BLANK_DEVICES_CONTROLLER_H

#include <QtCore>
#include <QtNetwork>
#include <QtBluetooth>

#include <QAbstractTableModel>

class KSQBlankDevicesController : public QAbstractTableModel {
    Q_OBJECT

    struct BlankDevInfo {
        VSQMac mac;
        QString type;
        QString name;
        QDateTime lastUpdate;

        BlankDevInfo(const VSQMac &_mac, QString _type, QString _name, const QDateTime &dt) {
            mac = _mac;
            type = _type;
            name = _name;
            lastUpdate = dt;
        }

        BlankDevInfo() {
            lastUpdate = QDateTime::currentDateTime().addYears(-1);
        }
    };

    typedef QMap<QString, BlankDevInfo> KSQBlankDevices;

public:
    enum Element { Name = Qt::UserRole, Type, Mac, ElementMax };

    KSQBlankDevicesController(QSharedPointer<VSQNetifBase> netif);

    KSQBlankDevicesController(KSQBlankDevicesController const &) = delete;

    KSQBlankDevicesController &
    operator=(KSQBlankDevicesController const &) = delete;

    virtual ~KSQBlankDevicesController() = default;

    Q_INVOKABLE bool
    provisionDevice(const QString &mac);

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

signals:

    void
    fireDeviceSelected(QBluetoothDeviceInfo dev) const;

private slots:
    void
    onDeviceInfoUpdate(const VSQDeviceInfo &deviceInfo);

    void
    cleanOldDevices();

private:
    static const int kInactiveTimeoutMS = 5000;

    QTimer m_cleanerTimer;

    QSharedPointer<VSQNetifBase> m_netif;


    KSQBlankDevices m_devices;
};

#endif // KSQ_BLANK_DEVICES_CONTROLLER_H
