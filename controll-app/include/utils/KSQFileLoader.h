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

#ifndef KSQ_FILELOADER_H
#define KSQ_FILELOADER_H

#include <QtQml>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLoggingCategory>

class FileLoader : public QObject {
    Q_OBJECT

public:
    FileLoader() = default;
    virtual ~FileLoader() = default;

    Q_INVOKABLE int
    download(const QString &url, const QString &file);

    Q_INVOKABLE int
    upload(const QString &url, const QString &file);

signals:
    void progressUpdated(qint64, qint64);
    void
    done();
    void
    error();

private:
    QMutex m_guard;
    QSharedPointer<QNetworkAccessManager> m_networkAccessManager;
    QSharedPointer<QFile> m_file;
};

static void
registerMetaTypes() {
    qmlRegisterType<FileLoader>("yiot.utils.loader", 1, 0, "KSFileLoader");
}
Q_COREAPP_STARTUP_FUNCTION(registerMetaTypes);

#endif // KSQ_FILELOADER_H
