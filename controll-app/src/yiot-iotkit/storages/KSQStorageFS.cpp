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
KSQStorageFS::KSQStorageFS(const QString &baseDir) : KSQStorageBase(kFileSizeMax) {
    m_baseDir = baseDir;
    QDir dir(m_baseDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    m_valid = dir.exists();

    VS_LOG_DEBUG("KSQStorageFS: %s", baseDir.toStdString().c_str());
}

//-----------------------------------------------------------------------------
bool
KSQStorageFS::writeImpl(const QString &file, const QByteArray &data) {
    CHECK_RET(m_valid, false, "Storage is not ready");

    deleteImpl(file);

    auto fn = m_baseDir + QDir::separator() + file;
    QFile f(fn);
    if (!f.open(QIODevice::WriteOnly)) {
        VS_LOG_WARNING("Cannot open file %s", fn.toStdString().c_str());
        return false;
    }

    bool res = data.size() == f.write(data);
    f.close();

    return res;
}

//-----------------------------------------------------------------------------
bool
KSQStorageFS::readImpl(const QString &file, QByteArray &data) {
    CHECK_RET(m_valid, false, "Storage is not ready");

    auto fn = m_baseDir + QDir::separator() + file;
    QFile f(fn);
    if (!f.open(QIODevice::ReadOnly)) {
#if 0
        VS_LOG_WARNING("Cannot open file %s", fn.toStdString().c_str());
#endif
        return false;
    }

    data = f.read(kFileSizeMax);
    f.close();

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQStorageFS::deleteImpl(const QString &file) {
    CHECK_RET(m_valid, false, "Storage is not ready");
    auto fn = m_baseDir + QDir::separator() + file;
    QFile::remove(fn);
    return true;
}

//-----------------------------------------------------------------------------
