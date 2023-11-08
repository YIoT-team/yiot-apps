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
FileLoader::download(const QString &url, const QString &file) {
    qDebug() << ">>>> FileLoader::download : " << url;
    return 0;
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
