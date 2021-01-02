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
#include <yiot-iotkit/secbox/KSQSecBox.h>

#include <yiot-iotkit/secmodule/KSQSecModule.h>

#include <virgil/iot/secbox/secbox.h>

using namespace VirgilIoTKit;

//-----------------------------------------------------------------------------
KSQSecBox::KSQSecBox() {
    m_valid = true;
    auto base = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation).first();
    m_storage = new KSQStorageFS(base + QDir::separator() + "secbox");

    auto storage = m_storage->storageImpl();
    auto secModule = KSQSecModule::instance().secmoduleImpl();
    if (VS_CODE_OK != vs_secbox_init(storage, secModule)) {
        VS_LOG_ERROR("Cannot initialize Security Box");
        return;
    }
}

//-----------------------------------------------------------------------------
KSQSecBox::~KSQSecBox() {
    delete m_storage;
}

//-----------------------------------------------------------------------------
bool
KSQSecBox::save(vs_secbox_type_t type, vs_storage_element_id_t id, const QByteArray &data) {
    Q_ASSERT(m_valid);
    return VS_CODE_OK ==
           vs_secbox_save(
                   type, reinterpret_cast<uint8_t *>(id), reinterpret_cast<const uint8_t *>(data.data()), data.size());
}

//-----------------------------------------------------------------------------
bool
KSQSecBox::load(vs_storage_element_id_t id, QByteArray &data) {
    Q_ASSERT(m_valid);
    data.resize(m_storage->kFileSizeMax);
    size_t dataSz = 0;
    if (VS_CODE_OK != vs_secbox_load(reinterpret_cast<uint8_t *>(id),
                                     reinterpret_cast<uint8_t *>(data.data()),
                                     m_storage->kFileSizeMax,
                                     &dataSz)) {
        return false;
    }

    data.resize(dataSz);
    return true;
}

//-----------------------------------------------------------------------------
bool
KSQSecBox::del(vs_storage_element_id_t id) {
    Q_ASSERT(m_valid);
    return VS_CODE_OK != vs_secbox_del(reinterpret_cast<uint8_t *>(id));
}

//-----------------------------------------------------------------------------
