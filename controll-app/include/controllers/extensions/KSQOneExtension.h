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

#ifndef KSQ_EXTENSION_INTEGRATION_H
#define KSQ_EXTENSION_INTEGRATION_H

#include <QtCore>

#include <QAbstractTableModel>

class KSQOneExtension : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString logo READ logo NOTIFY fireLogoChanged)
    Q_PROPERTY(QString name READ name NOTIFY fireNameChanged)
    Q_PROPERTY(QString version READ version NOTIFY fireVersionChanged)
    Q_PROPERTY(QString description READ description NOTIFY fireDescriptionChanged)
    Q_PROPERTY(int size READ size NOTIFY fireSizeChanged)
    Q_PROPERTY(QStringList lanuages READ lanuages NOTIFY fireLanguagesChanged)
    Q_PROPERTY(QString link READ link NOTIFY fireLinkChanged)
    Q_PROPERTY(QObject *js READ js NOTIFY fireJsChanged)
public:
    KSQOneExtension() = default;

    KSQOneExtension(const QString &a_logo,
                    const QString &a_name,
                    const QString &a_version,
                    const QString &a_descr,
                    const QString &a_link,
                    size_t a_size,
                    const QStringList a_languages,
                    bool a_isBuiltIn,
                    const QString &a_path);

    KSQOneExtension(KSQOneExtension const &);

    KSQOneExtension &
    operator=(KSQOneExtension const &);

    virtual ~KSQOneExtension() = default;

    QString
    logo() const;

    QString
    name() const;

    QString
    description() const;

    QString
    version() const;

    size_t
    size() const;

    QStringList
    lanuages() const;

    QString
    link() const;

    QObject *
    js();

    void
    setJs(QObject *js);

    bool
    isBuiltIn() const;

    QString
    basePath() const;

signals:
    void
    fireLogoChanged();

    void
    fireNameChanged();

    void
    fireVersionChanged();

    void
    fireDescriptionChanged();

    void
    fireSizeChanged();

    void
    fireLanguagesChanged();

    void
    fireLinkChanged();

    void
    fireJsChanged();

private slots:

private:
    QString m_logo;
    QString m_name;
    QString m_description;
    QString m_version;
    QString m_link;
    size_t m_size;
    QStringList m_languages;
    QObject *m_js;
    bool m_isBuiltIn;
    QString m_path;
};

Q_DECLARE_METATYPE(KSQOneExtension)
Q_DECLARE_METATYPE(KSQOneExtension *)
Q_DECLARE_METATYPE(const KSQOneExtension *)


#endif // KSQ_EXTENSION_INTEGRATION_H
