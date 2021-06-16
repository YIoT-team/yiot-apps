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

// #define PC_SERVER 1

#if PC_SERVER

#include <common/protocols/snap/user/user-server.h>
#include <common/protocols/snap/user/user-private.h>
#include <common/protocols/snap/user/user-structs.h>
#include <virgil/iot/protocols/snap/generated/snap_cvt.h>
#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/status_code/status_code.h>
#include <virgil/iot/logger/logger.h>
#include <virgil/iot/macros/macros.h>
#include <stdlib-config.h>
#include <endian-config.h>

static vs_snap_user_server_service_t _impl = {0};

//-----------------------------------------------------------------------------
static vs_status_e
_fill_current_state(const struct vs_netif_t *netif, char *json, const uint16_t json_buf_sz, uint16_t *json_sz) {
    vs_status_e res;

    if (_impl.get_data) {
        res = _impl.get_data(netif, json, json_buf_sz, json_sz);
        return res;
    }

    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
static vs_status_e
_get_pc_state_request_processor(const struct vs_netif_t *netif,
                                const vs_ethernet_header_t *eth_header,
                                const uint8_t *request,
                                const uint16_t request_sz,
                                uint8_t *response,
                                const uint16_t response_buf_sz,
                                uint16_t *response_sz) {
    return _fill_current_state(netif, (char *)response, response_buf_sz, response_sz);
}

//-----------------------------------------------------------------------------
static vs_status_e
_init_pc_cmd_request_processor(const struct vs_netif_t *netif,
                               const vs_ethernet_header_t *eth_header,
                               const uint8_t *request,
                               const uint16_t request_sz,
                               uint8_t *response,
                               const uint16_t response_buf_sz,
                               uint16_t *response_sz) {
    CHECK_NOT_ZERO_RET(eth_header, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(request, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_RET(request_sz < PC_JSON_SZ_MAX, VS_CODE_ERR_ZERO_ARGUMENT, "Request too long");

    uint16_t json_sz;
    const char *json = (const char *)request;
    json_sz = strnlen(json, request_sz);
    CHECK_RET(json_sz < request_sz, VS_CODE_ERR_INCORRECT_ARGUMENT, "Incorrect request");

    if (_impl.pc_cmd) {
        vs_status_e res = _impl.pc_cmd(netif, eth_header->src, json);
        if (VS_CODE_OK != res) {
            return res;
        }

        return _fill_current_state(netif, (char *)response, response_buf_sz, response_sz);
    }

    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
vs_status_e
vs_snap_user_start_notification(const vs_netif_t *netif) {
    if (!vs_provision_is_ready()) {
        return VS_CODE_COMMAND_NO_RESPONSE;
    }

    vs_status_e ret_code;
    char json[PC_JSON_SZ_MAX] = {0};
    uint16_t json_sz;

    STATUS_CHECK_RET(_fill_current_state(netif, json, PC_JSON_SZ_MAX, &json_sz), "Cannot fill PC state data");

    // Send request
    STATUS_CHECK_RET(vs_snap_send_request(
                             netif, vs_snap_broadcast_mac(), VS_PC_SERVICE_ID, VS_PC_IPST, (uint8_t *)&json, json_sz),
                     "Cannot send data");

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_pc_request_processor(const struct vs_netif_t *netif,
                      const vs_ethernet_header_t *eth_header,
                      vs_snap_element_t element_id,
                      const uint8_t *request,
                      const uint16_t request_sz,
                      uint8_t *response,
                      const uint16_t response_buf_sz,
                      uint16_t *response_sz) {
    (void)netif;

    *response_sz = 0;

    if (!vs_provision_is_ready()) {
        return VS_CODE_COMMAND_NO_RESPONSE;
    }

    switch (element_id) {
    case VS_PC_GPST:
        return _get_pc_state_request_processor(
                netif, eth_header, request, request_sz, response, response_buf_sz, response_sz);

    case VS_PC_PCMD:
        return _init_pc_cmd_request_processor(
                netif, eth_header, request, request_sz, response, response_buf_sz, response_sz);

    default:
        VS_LOG_ERROR("Unsupported PC command");
        VS_IOT_ASSERT(false);
        return VS_CODE_COMMAND_NO_RESPONSE;
    }
}

//-----------------------------------------------------------------------------
const vs_snap_service_t *
vs_snap_user_server(vs_snap_user_server_service_t impl) {

    static vs_snap_service_t _cfg;

    _cfg.user_data = NULL;
    _cfg.id = VS_PC_SERVICE_ID;
    _cfg.request_process = _pc_request_processor;
    _cfg.response_process = NULL;
    _cfg.periodical_process = NULL;

    // Save callbacks
    VS_IOT_MEMCPY(&_impl, &impl, sizeof(impl));

    return &_cfg;
}

//-----------------------------------------------------------------------------

#endif // PC_SERVER