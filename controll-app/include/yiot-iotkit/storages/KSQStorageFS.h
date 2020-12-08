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

#ifndef _YIOT_QT_FILE_SYSTEM_STORAGE_H_
#define _YIOT_QT_FILE_SYSTEM_STORAGE_H_

#include <QtCore>

#include <virgil/iot/storage_hal/storage_hal.h>

#include <yiot-iotkit/storages/KSQStorageBase.h>

using namespace VirgilIoTKit;

class KSQStorageFS final: public KSQStorageBase {

    Q_OBJECT

public:
    KSQStorageFS();
    virtual ~KSQStorageFS() = default;

protected:
    virtual bool
    openImpl(const QString & file);

    virtual bool
    closeImpl(const QString & file);

    virtual bool
    syncImpl(const QString & file);

    virtual bool
    writeImpl(const QString &file, size_t offset, const QByteArray &data);

    virtual bool
    readImpl(const QString &file, size_t offset, size_t readSz, QByteArray &data);

    virtual bool
    deleteImpl(const QString &file);

    virtual ssize_t
    fileSizeImpl(const QString &file);

private:
    static const size_t kFileSizeMax = 10 * 1024 * 1024;
};

#endif // _YIOT_QT_FILE_SYSTEM_STORAGE_H_
