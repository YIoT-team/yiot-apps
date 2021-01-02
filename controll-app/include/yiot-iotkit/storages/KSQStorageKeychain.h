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

#ifndef _YIOT_QT_KEYCHAIN_STORAGE_H_
#define _YIOT_QT_KEYCHAIN_STORAGE_H_

#include <QtCore>

#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/storage_hal/storage_hal.h>

#include <yiot-iotkit/storages/KSQStorageBase.h>

using namespace VirgilIoTKit;

class KSQStorageKeychain final : public KSQStorageBase, public VSQSingleton<KSQStorageKeychain> {

    Q_OBJECT

    friend VSQSingleton<KSQStorageKeychain>;

public:
protected:
    virtual bool
    writeImpl(const QString &file, const QByteArray &data);

    virtual bool
    readImpl(const QString &file, QByteArray &data);

    virtual bool
    deleteImpl(const QString &file);

private:
    static const size_t kFileSizeMax = 100 * 1024;
    static const QString kService;

    KSQStorageKeychain();
    virtual ~KSQStorageKeychain() = default;
};

#endif // _YIOT_QT_KEYCHAIN_STORAGE_H_
