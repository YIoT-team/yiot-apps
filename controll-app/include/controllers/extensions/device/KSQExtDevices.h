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

#ifndef KSQ_EXTENSION_DEVICES_H
#define KSQ_EXTENSION_DEVICES_H

#include <QtCore>
#include <QAbstractTableModel>

#include <controllers/extensions/device/KSQExtDevice.h>

class KSQExtDevices : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Element { Info = Qt::UserRole, ElementMax };

    KSQExtDevices();

    KSQExtDevices(KSQExtDevices const &) = delete;

    KSQExtDevices &
    operator=(KSQExtDevices const &) = delete;

    virtual ~KSQExtDevices() = default;

    QStringList
    builtInDevices() const;

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

private slots:

private:
    QList<QSharedPointer<KSQExtDevice>> m_devices;
    QStringList m_builtIn;

    QString
    fixQrcQFile(const QString &resourceDir);

    QString
    readContent(const QString &fileName);

    bool
    loadBuiltinDevicesInfo();

    bool
    loadOneBuiltinDevice(const QString &resourceDir);
};

#endif // KSQ_EXTENSION_DEVICES_H
