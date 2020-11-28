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

#if PC_SERVER

#include <common/protocols/snap/pc/pc-server.h>
#include <common/protocols/snap/pc/pc-private.h>
#include <common/protocols/snap/pc/pc-structs.h>
#include <virgil/iot/protocols/snap/generated/snap_cvt.h>
#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/status_code/status_code.h>
#include <virgil/iot/logger/logger.h>
#include <virgil/iot/macros/macros.h>
#include <stdlib-config.h>
#include <endian-config.h>

static vs_snap_pc_server_service_t _impl = {0};

//-----------------------------------------------------------------------------
static vs_status_e
_fill_current_state(const struct vs_netif_t *netif,
                    const vs_ethernet_header_t *eth_header,
                    uint8_t *response,
                    const uint16_t response_buf_sz,
                    uint16_t *response_sz) {
    vs_status_e res;
    CHECK_NOT_ZERO_RET(eth_header, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_RET(response_buf_sz >= sizeof(vs_snap_pc_state_t), VS_CODE_ERR_TOO_SMALL_BUFFER, "Small buffer");

    if (_impl.get_data) {
        vs_snap_pc_state_t *state = (vs_snap_pc_state_t *)response;
        res = _impl.get_data(netif, eth_header->src, state);

        if (VS_CODE_OK == res) {
            // TODO: Normalize byte order
            *response_sz = sizeof(vs_snap_pc_state_t);
        }
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
    return _fill_current_state(netif, eth_header, response, response_buf_sz, response_sz);
}

//-----------------------------------------------------------------------------
static vs_status_e
_init_pc_ssh_request_processor(const struct vs_netif_t *netif,
                               const vs_ethernet_header_t *eth_header,
                               const uint8_t *request,
                               const uint16_t request_sz,
                               uint8_t *response,
                               const uint16_t response_buf_sz,
                               uint16_t *response_sz) {
    CHECK_NOT_ZERO_RET(eth_header, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(request, VS_CODE_ERR_ZERO_ARGUMENT);

    if (_impl.init_pc_ssh) {
        vs_snap_pc_init_ssh_t *init = (vs_snap_pc_init_ssh_t *)request;

        if (VS_CODE_OK != _impl.init_pc_ssh(netif, eth_header->src, init)) {
            return VS_CODE_ERR_UNSUPPORTED;
        }

        return _fill_current_state(netif, eth_header, response, response_buf_sz, response_sz);
    }

    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
static vs_status_e
_init_pc_vpn_request_processor(const struct vs_netif_t *netif,
                               const vs_ethernet_header_t *eth_header,
                               const uint8_t *request,
                               const uint16_t request_sz,
                               uint8_t *response,
                               const uint16_t response_buf_sz,
                               uint16_t *response_sz) {
    CHECK_NOT_ZERO_RET(eth_header, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(request, VS_CODE_ERR_ZERO_ARGUMENT);

    if (_impl.init_pc_vpn) {
        vs_snap_pc_init_vpn_t *init = (vs_snap_pc_init_vpn_t *)request;

        if (VS_CODE_OK != _impl.init_pc_vpn(netif, eth_header->src, init)) {
            return VS_CODE_ERR_UNSUPPORTED;
        }

        return _fill_current_state(netif, eth_header, response, response_buf_sz, response_sz);
    }

    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
vs_status_e
vs_snap_pc_start_notification(const vs_netif_t *netif) {

    vs_snap_pc_state_t state_data;
    vs_status_e ret_code;


    uint16_t request_sz = 0;
    vs_ethernet_header_t eth_header;
    memset(&eth_header, 0xFF, sizeof(eth_header));
    STATUS_CHECK_RET(_fill_current_state(netif, &eth_header, (uint8_t *)&state_data, sizeof(state_data), &request_sz),
                     "Cannot fill PC state data");

    // Send request
    STATUS_CHECK_RET(vs_snap_send_request(netif,
                                          vs_snap_broadcast_mac(),
                                          VS_PC_SERVICE_ID,
                                          VS_PC_IPST,
                                          (uint8_t *)&state_data,
                                          sizeof(state_data)),
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

    switch (element_id) {
    case VS_PC_GPST:
        return _get_pc_state_request_processor(
                netif, eth_header, request, request_sz, response, response_buf_sz, response_sz);

    case VS_PC_ISSH:
        return _init_pc_ssh_request_processor(
                netif, eth_header, request, request_sz, response, response_buf_sz, response_sz);

    case VS_PC_IVPN:
        return _init_pc_vpn_request_processor(
                netif, eth_header, request, request_sz, response, response_buf_sz, response_sz);

    default:
        VS_LOG_ERROR("Unsupported PC command");
        VS_IOT_ASSERT(false);
        return VS_CODE_COMMAND_NO_RESPONSE;
    }
}

//-----------------------------------------------------------------------------
const vs_snap_service_t *
vs_snap_pc_server(vs_snap_pc_server_service_t impl) {

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