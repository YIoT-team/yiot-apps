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
#include <yiot-iotkit/storages/KSQStorageBase.h>

//-----------------------------------------------------------------------------
KSQStorageBase::KSQStorageBase(size_t fileSizeMax) {
    memset(&m_storageImpl, 0, sizeof(m_storageImpl));

    m_storageImpl.file_sz_limit = fileSizeMax;
    m_storageImpl.impl_func = _funcImpl();
    m_storageImpl.impl_data = this;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_deinit(vs_storage_impl_data_ctx_t storage_ctx) {
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_INCORRECT_PARAMETER);
    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
QString
KSQStorageBase::id2str(vs_storage_element_id_t id) {
    return "";
}

//-----------------------------------------------------------------------------
vs_storage_file_t
KSQStorageBase::_open(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_element_id_t id) {
    auto storage = (KSQStorageBase *)storage_ctx;

    CHECK_NOT_ZERO_RET(id, NULL);
    CHECK_NOT_ZERO_RET(storage_ctx, NULL);

    uint32_t len = sizeof(vs_storage_element_id_t) * 2 + 1;
    uint8_t *file = (uint8_t *)VS_IOT_CALLOC(1, len);
    CHECK_NOT_ZERO_RET(file, NULL);

    _data_to_hex(id, sizeof(vs_storage_element_id_t), file, &len);

    return file;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_sync(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_file_t file) {
    vs_status_e res = VS_CODE_ERR_FILE;

    CHECK_NOT_ZERO_RET(file, VS_CODE_ERR_NULLPTR_ARGUMENT);
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_NULLPTR_ARGUMENT);
    vs_nix_storage_ctx_t *ctx = (vs_nix_storage_ctx_t *)storage_ctx;

    if (vs_files_sync(ctx->dir, (char *)file)) {
        res = VS_CODE_OK;
    }
    return res;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_close(const vs_storage_impl_data_ctx_t storage_ctx, vs_storage_file_t file) {
    CHECK_NOT_ZERO_RET(file, VS_CODE_ERR_INCORRECT_PARAMETER);

    VS_IOT_FREE(file);

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_save(const vs_storage_impl_data_ctx_t storage_ctx,
                        const vs_storage_file_t file,
                        size_t offset,
                        const uint8_t *data,
                        size_t data_sz) {
    vs_status_e res = VS_CODE_ERR_FILE_WRITE;

    CHECK_NOT_ZERO_RET(data, VS_CODE_ERR_NULLPTR_ARGUMENT);
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_NULLPTR_ARGUMENT);
    CHECK_NOT_ZERO_RET(file, VS_CODE_ERR_NULLPTR_ARGUMENT);
    vs_nix_storage_ctx_t *ctx = (vs_nix_storage_ctx_t *)storage_ctx;

    if (vs_files_write(ctx->dir, (char *)file, offset, data, data_sz)) {
        res = VS_CODE_OK;
    }
    return res;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_load(const vs_storage_impl_data_ctx_t storage_ctx,
                        const vs_storage_file_t file,
                        size_t offset,
                        uint8_t *out_data,
                        size_t data_sz) {
    size_t read_sz;
    vs_status_e res = VS_CODE_ERR_FILE_READ;
    vs_nix_storage_ctx_t *ctx = (vs_nix_storage_ctx_t *)storage_ctx;

    CHECK_NOT_ZERO_RET(out_data, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(file, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(ctx->dir, VS_CODE_ERR_INCORRECT_PARAMETER);

    if (vs_files_read(ctx->dir, (char *)file, offset, out_data, data_sz, &read_sz) && read_sz == data_sz) {
        res = VS_CODE_OK;
    }

    return res;
}

//-----------------------------------------------------------------------------
ssize_t
KSQStorageBase::_fileSize(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_element_id_t id) {
    vs_nix_storage_ctx_t *ctx = (vs_nix_storage_ctx_t *)storage_ctx;
    ssize_t res;
    uint32_t len = sizeof(vs_storage_element_id_t) * 2 + 1;
    uint8_t file[len];

    CHECK_NOT_ZERO_RET(id, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(ctx->dir, VS_CODE_ERR_INCORRECT_PARAMETER);

    _data_to_hex(id, sizeof(vs_storage_element_id_t), file, &len);
    res = vs_files_get_len(ctx->dir, (char *)file);
    return res;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_del(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_element_id_t id) {
    vs_nix_storage_ctx_t *ctx = (vs_nix_storage_ctx_t *)storage_ctx;

    CHECK_NOT_ZERO_RET(id, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(ctx->dir, VS_CODE_ERR_INCORRECT_PARAMETER);

    uint32_t len = sizeof(vs_storage_element_id_t) * 2 + 1;
    uint8_t file[len];

    _data_to_hex(id, sizeof(vs_storage_element_id_t), file, &len);

    return vs_files_remove(ctx->dir, (char *)file) ? VS_CODE_OK : VS_CODE_ERR_FILE_DELETE;
}

//-----------------------------------------------------------------------------
vs_storage_impl_func_t
KSQStorageBase::_funcImpl() {
    vs_storage_impl_func_t impl;
    memset(&impl, 0, sizeof(impl));

    impl.size = _fileSize;
    impl.deinit = _deinit;
    impl.open = _open;
    impl.sync = _sync;
    impl.close = _close;
    impl.save = _save;
    impl.load = _load;
    impl.del = _del;

    return impl;
}

//-----------------------------------------------------------------------------
