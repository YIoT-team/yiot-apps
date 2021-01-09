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

#ifndef YIOT_ROOT_OF_TRUST_LIST_H
#define YIOT_ROOT_OF_TRUST_LIST_H

#include <QtCore>
#include <QHash>
#include <QAbstractTableModel>

#include <virgil/iot/qt/VSQIoTKit.h>
#include <yiot-iotkit/root-of-trust/KSQRoT.h>

#include <virgil/iot/qt/helpers/VSQSingleton.h>

class KSQRoTController : public QAbstractTableModel, public VSQSingleton<KSQRoTController> {
    Q_OBJECT

    friend VSQSingleton<KSQRoTController>;
    enum Element {
        ID = Qt::UserRole,
        Name,
        Image,
        TrustList,
        ECType,
        Recovery1,
        Recovery2,
        Auth1,
        Auth2,
        TL1,
        TL2,
        Firmware1,
        Firmware2,
        ElementMax
    };

public:
    bool
    isValid() const {
        return m_valid;
    }

    QSharedPointer<KSQRoT>
    localRootOfTrust() const;

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

signals:

private:
    KSQRoTController();
    virtual ~KSQRoTController() = default;

    std::list<QSharedPointer<KSQRoT>> m_rots;
    bool m_valid;

    bool
    prepare();

    QStringList
    loadRoTList();

    bool
    saveRoTList(const QStringList &list);

    vs_storage_element_id_t m_listStorageId;
};

#endif // YIOT_ROOT_OF_TRUST_LIST_H
