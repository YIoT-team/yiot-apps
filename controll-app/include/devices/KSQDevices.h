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

#ifndef YIOT_DEVICES_H
#define YIOT_DEVICES_H

#include <QtCore>
#include <QHash>
#include <QAbstractTableModel>

#include <virgil/iot/qt/VSQIoTKit.h>

#include <devices/KSQControllerBase.h>
#include <devices/KSQDeviceBase.h>

class KSQDevices : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Element { Name = Qt::UserRole, Type, Image, SubModel, ElementMax };

    KSQDevices() = default;
    virtual ~KSQDevices() = default;

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

    KSQDevices &
    operator<<(KSQControllerBase *controller);

signals:
    void
    fireNewProvisionedDevice(QSharedPointer<KSQDeviceBase> device);

public slots:

private slots:
    void
    onGroupActivated();

private:
    QList<QSharedPointer<KSQControllerBase>> m_elements;
};

#endif // YIOT_DEVICES_H
