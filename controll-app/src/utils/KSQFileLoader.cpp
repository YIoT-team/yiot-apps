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

#include "utils/KSQFileLoader.h"

#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkRequest>

// ----------------------------------------------------------------------------
int
FileLoader::download(const QString &url) {
    qDebug() << "FileLoader::download : " << url;
    QMutexLocker _lock(&m_guard);

    QUrl fullUrl(url);
    fullUrl.setUserName("anonymous");
    fullUrl.setPassword("anonymous");

    m_networkAccessManager.reset(new QNetworkAccessManager());
    auto reply = m_networkAccessManager->get(QNetworkRequest(fullUrl));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), SIGNAL(progressUpdated(qint64, qint64)));
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SIGNAL(error()));

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit error();
        }

        m_data = reply->readAll();
        emit done();
    });

    return 0;
}

// ----------------------------------------------------------------------------
int
FileLoader::save(const QString &file) {
    qDebug() << "FileLoader::save : " << file;

    auto f = QFile(file);
    if (!f.open(QFile::WriteOnly)) {
        return -1;
    }

    auto res = f.write(m_data);
    return res ? 0 : -1;
}

// ----------------------------------------------------------------------------
int
FileLoader::upload(const QString &url, const QString &file) {
    qDebug() << "FileLoader::upload : " << url << " -> " << file;

    QMutexLocker _lock(&m_guard);

    QUrl fullUrl(url);
    fullUrl.setUserName("anonymous");
    fullUrl.setPassword("anonymous");

    m_networkAccessManager.reset(new QNetworkAccessManager());
    m_file.reset(new QFile(file));
    if (m_file->open(QIODevice::ReadOnly)) {
        auto reply = m_networkAccessManager->put(QNetworkRequest(fullUrl), m_file.get());
        connect(reply, SIGNAL(uploadProgress(qint64, qint64)), SIGNAL(progressUpdated(qint64, qint64)));
        connect(reply, SIGNAL(finished()), SIGNAL(done()));
        connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SIGNAL(error()));
    } else {
        emit error();
    }

    return 0;
}

// ----------------------------------------------------------------------------
