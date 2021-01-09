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

#include <yiot-iotkit/storages/KSQStorageKeychain.h>

#include "keychain.h"

const QString KSQStorageKeychain::kService = "YIoT";

//-----------------------------------------------------------------------------
KSQStorageKeychain::KSQStorageKeychain() : KSQStorageBase(kFileSizeMax) {
}

//-----------------------------------------------------------------------------
bool
KSQStorageKeychain::writeImpl(const QString &file, const QByteArray &data) {
    QKeychain::WritePasswordJob job(kService);
    job.setAutoDelete(false);
    job.setKey(file);
    job.setBinaryData(data);

    QEventLoop loop;
    job.connect(&job, SIGNAL(finished(QKeychain::Job *)), &loop, SLOT(quit()));
    job.start();
    loop.exec();

    if (job.error()) {
        VS_LOG_WARNING("Keychain: Storing data failed for file %s, error text: %s",
                       file.toStdString().c_str(),
                       job.errorString().toStdString().c_str());
        return false;
    }

    VS_LOG_DEBUG("Keychain: Data stored successfully for file %s", file.toStdString().c_str());
    return true;
}

//-----------------------------------------------------------------------------
bool
KSQStorageKeychain::readImpl(const QString &file, QByteArray &data) {
    QKeychain::ReadPasswordJob job(kService);
    job.setAutoDelete(false);
    job.setKey(file);

    QEventLoop loop;
    job.connect(&job, SIGNAL(finished(QKeychain::Job *)), &loop, SLOT(quit()));
    job.start();
    loop.exec();

    data = job.binaryData();

    if (job.error()) {
        VS_LOG_WARNING("Keychain: Loading data failed for file %s, error text: %s",
                       file.toStdString().c_str(),
                       job.errorString().toStdString().c_str());
        return false;
    }

    VS_LOG_DEBUG("Keychain: Data loaded successfully for file %s", file.toStdString().c_str());

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQStorageKeychain::deleteImpl(const QString &file) {
    QKeychain::DeletePasswordJob job(kService);
    job.setAutoDelete(false);
    job.setKey(file);

    QEventLoop loop;
    job.connect(&job, SIGNAL(finished(QKeychain::Job *)), &loop, SLOT(quit()));
    job.start();
    loop.exec();

    if (job.error()) {
        VS_LOG_WARNING("Keychain: Deleting data failed for file %s, error text: %s",
                       file.toStdString().c_str(),
                       job.errorString().toStdString().c_str());
        return false;
    }

    VS_LOG_DEBUG("Keychain: Data deleted successfully for file %s", file.toStdString().c_str());

    return true;
}

//-----------------------------------------------------------------------------
