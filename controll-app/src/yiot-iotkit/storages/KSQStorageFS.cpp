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

#include <virgil/iot/qt/VSQIoTKit.h>
#include <yiot-iotkit/storages/KSQStorageFS.h>

//-----------------------------------------------------------------------------
KSQStorageFS::KSQStorageFS() : KSQStorageBase(kFileSizeMax) {

}

//-----------------------------------------------------------------------------
bool
KSQStorageFS::openImpl(const QString & file) {
    return false;
}

//-----------------------------------------------------------------------------
bool
KSQStorageFS::closeImpl(const QString & file) {
    return false;
}

//-----------------------------------------------------------------------------
bool
KSQStorageFS::syncImpl(const QString & file) {
    return false;
}

//-----------------------------------------------------------------------------
bool
KSQStorageFS::writeImpl(const QString &file, size_t offset, const QByteArray &data) {
    return false;
}

//-----------------------------------------------------------------------------
bool
KSQStorageFS::readImpl(const QString &file, size_t offset, size_t readSz, QByteArray &data) {
    return false;
}

//-----------------------------------------------------------------------------
bool
KSQStorageFS::deleteImpl(const QString &file) {
    return false;
}

//-----------------------------------------------------------------------------
ssize_t
KSQStorageFS::fileSizeImpl(const QString &file) {
    return false;
}

//-----------------------------------------------------------------------------
