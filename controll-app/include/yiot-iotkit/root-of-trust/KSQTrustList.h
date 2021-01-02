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

#ifndef YIOT_ROT_TRUSTLIST_H
#define YIOT_ROT_TRUSTLIST_H

#include <QtCore>

#include <yiot-iotkit/secmodule/KSQSecModule.h>
#include <yiot-iotkit/secbox/KSQSecBox.h>

class KSQRoT;

class KSQTrustList : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString version READ version)
    Q_PROPERTY(int keysCount READ keysCount)
public:
    KSQTrustList(const QString &id = "");

    KSQTrustList(const KSQTrustList &tl);

    virtual ~KSQTrustList() = default;

    const QByteArray &
    val() const {
        return m_tl;
    }

    bool
    isValid() const {
        return m_valid;
    }

    QString
    version() const;

    int
    keysCount() const;

    bool
    create(const QString &id, const KSQRoT &rot);

    bool
    load(const QString &id);

    int
    minSz() const;

    QByteArray
    header() const;

    QByteArray
    key(size_t num) const;

    QByteArray
    footer() const;

signals:

public slots:

private:
    bool m_valid;
    QString m_id;
    QByteArray m_tl;

    bool
    save() const;

    bool
    fillStorageId(vs_storage_element_id_t id) const;
};

Q_DECLARE_METATYPE(KSQTrustList)
Q_DECLARE_METATYPE(KSQTrustList *)

#endif // YIOT_ROT_TRUSTLIST_H
