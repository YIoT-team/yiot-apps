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

#if LAMP_SERVER

#include <virgil/iot/protocols/snap/lamp/lamp-server.h>
#include <virgil/iot/protocols/snap/lamp/lamp-private.h>
#include <virgil/iot/protocols/snap/lamp/lamp-structs.h>
#include <virgil/iot/protocols/snap/generated/snap_cvt.h>
#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/status_code/status_code.h>
#include <virgil/iot/logger/logger.h>
#include <virgil/iot/macros/macros.h>
#include <stdlib-config.h>
#include <endian-config.h>

static vs_snap_lamp_server_service_t _impl = {0};

//-----------------------------------------------------------------------------
static vs_status_e
_fill_current_state(uint8_t *response, const uint16_t response_buf_sz, uint16_t *response_sz) {
    vs_status_e res;
    CHECK_RET(response_buf_sz >= sizeof(vs_snap_lamp_state_t), VS_CODE_ERR_TOO_SMALL_BUFFER, "Small buffer");
    if (_impl.get_data) {
        vs_snap_lamp_state_t *state = (vs_snap_lamp_state_t *)response;
        res = _impl.get_data(state);

        if (VS_CODE_OK == res) {
            // TODO: Normalize byte order
            *response_sz = sizeof(vs_snap_lamp_state_t);
        }
        return res;
    }

    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
static vs_status_e
_get_lamp_state_request_processor(const uint8_t *request,
                                  const uint16_t request_sz,
                                  uint8_t *response,
                                  const uint16_t response_buf_sz,
                                  uint16_t *response_sz) {
    return _fill_current_state(response, response_buf_sz, response_sz);
}

//-----------------------------------------------------------------------------
static vs_status_e
_lamp_request_processor(const struct vs_netif_t *netif,
                        vs_snap_element_t element_id,
                        const uint8_t *request,
                        const uint16_t request_sz,
                        uint8_t *response,
                        const uint16_t response_buf_sz,
                        uint16_t *response_sz) {
    (void)netif;

    *response_sz = 0;

    switch (element_id) {
    case VS_LAMP_GLST:
        return _get_lamp_state_request_processor(request, request_sz, response, response_buf_sz, response_sz);

    case VS_LAMP_SLST:
        return _set_lamp_state_request_processor(request, request_sz, response, response_buf_sz, response_sz);

    default:
        VS_LOG_ERROR("Unsupported LAMP command");
        VS_IOT_ASSERT(false);
        return VS_CODE_COMMAND_NO_RESPONSE;
    }
}

//-----------------------------------------------------------------------------
const vs_snap_service_t *
vs_snap_lamp_server(vs_snap_lamp_server_service_t impl) {

    static vs_snap_service_t _cfg;

    _cfg.user_data = NULL;
    _cfg.id = VS_LAMP_SERVICE_ID;
    _cfg.request_process = _lamp_request_processor;
    _cfg.response_process = NULL;
    _cfg.periodical_process = NULL;

    // Save callbacks
    VS_IOT_MEMCPY(&_impl, &impl, sizeof(impl));

    return &_cfg;
}

//-----------------------------------------------------------------------------

#endif // LAMP_SERVER