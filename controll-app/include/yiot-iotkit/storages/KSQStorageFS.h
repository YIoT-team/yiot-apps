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

using namespace VirgilIoTKit;

class KSQStorageFS : public QObject {

    Q_OBJECT

public:
    KSQStorageFS();
    virtual ~KSQStorageFS() = default;

    VirgilIoTKit::vs_storage_op_ctx_t *
    storageImpl() {
        return &m_storageImpl;
    }

signals:

public slots:

private:
    static const size_t kFileSizeMax = 10 * 1024 * 1024;
    vs_storage_op_ctx_t m_storageImpl;

    vs_storage_impl_func_t
    _funcImpl();

    static ssize_t
    _fileSize(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_element_id_t id);

    static vs_status_e
    _deinit(vs_storage_impl_data_ctx_t storage_ctx);

    static vs_storage_file_t
    _open(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_element_id_t id);

    static vs_status_e
    _sync(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_file_t file);

    static vs_status_e
    _close(const vs_storage_impl_data_ctx_t storage_ctx, vs_storage_file_t file);

    static vs_status_e
    _save(const vs_storage_impl_data_ctx_t storage_ctx,
          const vs_storage_file_t file,
          size_t offset,
          const uint8_t *data,
          size_t data_sz);

    static vs_status_e
    _load(const vs_storage_impl_data_ctx_t storage_ctx,
          const vs_storage_file_t file,
          size_t offset,
          uint8_t *out_data,
          size_t data_sz);

    static vs_status_e
    _del(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_element_id_t id);
};

#endif // _YIOT_QT_FILE_SYSTEM_STORAGE_H_
