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
#include <virgil/iot/qt/helpers/VSQMac.h>

#include <yiot-iotkit/secmodule/KSQSessionKey.h>

#include <common/protocols/snap/user/user-structs.h>
#include <controllers/devices/KSQDevicesType.h>
#include <controllers/extensions/KSQExtensionControllerBase.h>

class KSQAllDevicesController : public QAbstractTableModel, public KSQExtensionControllerBase {
    Q_OBJECT
public:
    enum Element { Name = Qt::UserRole, Type, Image, SubModel, Js, ElementMax };

    KSQAllDevicesController() = default;
    virtual ~KSQAllDevicesController() = default;

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
    fireNewProvisionedDevice(QSharedPointer<KSQDevice> device);

    void
    fireNewUnknownDevice();

    void
    fireSessionKeyReceived(KSQDevice *);

public slots:

protected:
    virtual bool
    load(QSharedPointer<KSQOneExtension> extension) override final;

private slots:
    void
    onGroupActivated();

private:
    QList<QSharedPointer<KSQDevicesType>> m_elements;

    void
    add(KSQDevicesType *devicesType);
};

#endif // YIOT_DEVICES_H
