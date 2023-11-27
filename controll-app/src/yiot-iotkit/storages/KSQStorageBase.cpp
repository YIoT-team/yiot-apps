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
KSQStorageBase::id2str(const vs_storage_element_id_t id) {
    QString res;
    for (int i = 0; i < VS_STORAGE_ELEMENT_ID_MAX; i++) {
        auto c = QChar(id[i]);
        if (c.isLetterOrNumber() || c == '_' || c == '-') {
            res += c;
        } else {
            auto baHex = QByteArray(reinterpret_cast<const char *>(&id[i]), 1).toHex();
            res += QString(baHex);
        }
    }
    return res;
}

//-----------------------------------------------------------------------------
vs_storage_file_t
KSQStorageBase::_open(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_element_id_t id) {
    CHECK_NOT_ZERO_RET(id, NULL);
    CHECK_NOT_ZERO_RET(storage_ctx, NULL);

    auto f = id2str(id);
    auto ctx = reinterpret_cast<KSQStorageBase *>(storage_ctx);

    QByteArray baData;
    if (ctx->readImpl(f, baData)) {
        ctx->m_cache[f] = baData;
    } else {
        ctx->m_cache[f] = QByteArray();
    }

    return new QString(f);
}

//-----------------------------------------------------------------------------
bool
KSQStorageBase::_inCache(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_file_t file) {
    const auto &f = *reinterpret_cast<QString *>(file);
    auto ctx = reinterpret_cast<KSQStorageBase *>(storage_ctx);
    return ctx->m_cache.keys().contains(f);
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_close(const vs_storage_impl_data_ctx_t storage_ctx, vs_storage_file_t file) {
    CHECK_NOT_ZERO_RET(file, VS_CODE_ERR_INCORRECT_PARAMETER);

    auto ctx = reinterpret_cast<KSQStorageBase *>(storage_ctx);
    auto f = reinterpret_cast<QString *>(file);
    ctx->m_cache.remove(*f);
    delete f;

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_sync(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_file_t file) {
    vs_status_e res = VS_CODE_ERR_FILE;

    CHECK_NOT_ZERO_RET(file, VS_CODE_ERR_NULLPTR_ARGUMENT);
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_NULLPTR_ARGUMENT);
    CHECK_NOT_ZERO_RET(_inCache(storage_ctx, file), VS_CODE_ERR_FILE);

    auto ctx = reinterpret_cast<KSQStorageBase *>(storage_ctx);
    const auto &f = *reinterpret_cast<QString *>(file);

    if (ctx->writeImpl(f, ctx->m_cache[f])) {
        res = VS_CODE_OK;
    }
    return res;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_save(const vs_storage_impl_data_ctx_t storage_ctx,
                      const vs_storage_file_t file,
                      size_t offset,
                      const uint8_t *data,
                      size_t data_sz) {
    CHECK_NOT_ZERO_RET(data, VS_CODE_ERR_NULLPTR_ARGUMENT);
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_NULLPTR_ARGUMENT);
    CHECK_NOT_ZERO_RET(file, VS_CODE_ERR_NULLPTR_ARGUMENT);
    CHECK_NOT_ZERO_RET(_inCache(storage_ctx, file), VS_CODE_ERR_FILE);

    auto ctx = reinterpret_cast<KSQStorageBase *>(storage_ctx);
    const auto &f = *reinterpret_cast<QString *>(file);

    QByteArray &fileData = ctx->m_cache[f];

    if (fileData.size() < static_cast<int>(offset + data_sz)) {
        fileData.resize(offset + data_sz);
    }

    memcpy(fileData.data() + offset, data, data_sz);

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_load(const vs_storage_impl_data_ctx_t storage_ctx,
                      const vs_storage_file_t file,
                      size_t offset,
                      uint8_t *out_data,
                      size_t data_sz) {
    vs_status_e res = VS_CODE_ERR_FILE_READ;

    CHECK_NOT_ZERO_RET(out_data, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(file, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(_inCache(storage_ctx, file), VS_CODE_ERR_FILE);

    auto ctx = reinterpret_cast<KSQStorageBase *>(storage_ctx);
    const auto &f = *reinterpret_cast<QString *>(file);
    QByteArray &fileData = ctx->m_cache[f];

    if (fileData.size() < static_cast<int>(offset + data_sz)) {
        return res;
    }

    memcpy(out_data, fileData.data() + offset, data_sz);

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
ssize_t
KSQStorageBase::_fileSize(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_element_id_t id) {
    CHECK_NOT_ZERO_RET(id, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_INCORRECT_PARAMETER);

    auto f = id2str(id);
    auto ctx = reinterpret_cast<KSQStorageBase *>(storage_ctx);

    QByteArray baData;
    if (ctx->readImpl(f, baData)) {
        ctx->m_cache[f] = baData;
    } else {
        ctx->m_cache[f] = QByteArray();
    }

    return baData.size();
}

//-----------------------------------------------------------------------------
vs_status_e
KSQStorageBase::_del(const vs_storage_impl_data_ctx_t storage_ctx, const vs_storage_element_id_t id) {
    vs_status_e res = VS_CODE_ERR_FILE_DELETE;

    CHECK_NOT_ZERO_RET(id, VS_CODE_ERR_INCORRECT_PARAMETER);
    CHECK_NOT_ZERO_RET(storage_ctx, VS_CODE_ERR_INCORRECT_PARAMETER);

    auto ctx = reinterpret_cast<KSQStorageBase *>(storage_ctx);

    if (ctx->deleteImpl(id2str(id))) {
        res = VS_CODE_OK;
    }

    return res;
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
