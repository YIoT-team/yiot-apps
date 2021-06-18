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

#ifndef KSQ_EXTENSION_DEVICE_H
#define KSQ_EXTENSION_DEVICE_H

#include <QtCore>

#include <QAbstractTableModel>

class KSQExtDevice : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString logo READ logo)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString version READ version)
    Q_PROPERTY(QString description READ description)
public:
    KSQExtDevice() = default;

    KSQExtDevice(const QString &a_logo, const QString &a_name, const QString &a_version, const QString &a_descr);

    KSQExtDevice(KSQExtDevice const &);

    KSQExtDevice &
    operator=(KSQExtDevice const &);

    virtual ~KSQExtDevice() = default;

    QString
    logo() const;

    QString
    name() const;

    QString
    description() const;

    QString
    version() const;

signals:

private slots:

private:
    QString m_logo;
    QString m_name;
    QString m_description;
    QString m_version;
};

Q_DECLARE_METATYPE(KSQExtDevice)
Q_DECLARE_METATYPE(KSQExtDevice *)
Q_DECLARE_METATYPE(const KSQExtDevice *)

#endif // KSQ_EXTENSION_DEVICE_H
