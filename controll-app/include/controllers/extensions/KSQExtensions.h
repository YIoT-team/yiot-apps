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

#ifndef KSQ_EXTENSION_INTEGRATIONS_H
#define KSQ_EXTENSION_INTEGRATIONS_H

#include <QtCore>
#include <QAbstractTableModel>

#include <controllers/extensions/KSQOneExtension.h>
#include <controllers/extensions/KSQExtensionControllerBase.h>

class KSQExtensions : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Element { Info = Qt::UserRole, Js, ElementMax };

    KSQExtensions(const QString &prefix, QSharedPointer<KSQExtensionControllerBase> processor);

    KSQExtensions(KSQExtensions const &) = delete;

    KSQExtensions &
    operator=(KSQExtensions const &) = delete;

    virtual ~KSQExtensions() = default;

    bool
    load();

    QList<QSharedPointer<KSQOneExtension>>
    builtInExtensions() const;

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
    QString m_prefix;
    QList<QSharedPointer<KSQOneExtension>> m_extensions;
    QSharedPointer<KSQExtensionControllerBase> m_processor;

    QString
    fixQrcQFile(const QString &resourceDir);

    QString
    readContent(const QString &fileName);

    bool
    loadBuiltinExtensions();

    bool
    loadOneExtension(const QString &resourceDir);
};

#endif // KSQ_EXTENSION_INTEGRATIONS_H
