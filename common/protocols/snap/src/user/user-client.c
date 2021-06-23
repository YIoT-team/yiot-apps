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

#if PC_CLIENT

#include <common/protocols/snap/user/user-client.h>
#include <common/protocols/snap/user/user-private.h>
#include <virgil/iot/protocols/snap/generated/snap_cvt.h>
#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/status_code/status_code.h>
#include <virgil/iot/macros/macros.h>
#include <virgil/iot/logger/logger.h>
#include <stdlib-config.h>
#include <global-hal.h>

// External functions for access to upper level implementations
static vs_snap_service_t _pc_client = {0};
static vs_snap_user_client_service_t _impl = {0};

//-----------------------------------------------------------------------------
vs_status_e
vs_snap_user_get_state(const vs_netif_t *netif, const vs_mac_addr_t *mac) {
    const vs_mac_addr_t *dst_mac;
    vs_status_e ret_code;

    // Set destination mac
    dst_mac = mac ? mac : vs_snap_broadcast_mac();

    // Send request
    STATUS_CHECK_RET(vs_snap_send_request(netif, dst_mac, VS_PC_SERVICE_ID, VS_PC_GPST, NULL, 0),
                     "Cannot send request");

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_status_e
vs_snap_user_command(const vs_netif_t *netif, const vs_mac_addr_t *mac, const char *json) {
    vs_status_e ret_code;
    uint16_t sz;

    // Check input parameters
    CHECK_NOT_ZERO_RET(mac, VS_CODE_ERR_INCORRECT_ARGUMENT);
    CHECK_NOT_ZERO_RET(json, VS_CODE_ERR_INCORRECT_ARGUMENT);
    sz = strnlen(json, PC_JSON_SZ_MAX);
    CHECK_RET(sz < PC_JSON_SZ_MAX, VS_CODE_ERR_INCORRECT_ARGUMENT, "JSON command too long");
    ++sz;


    // Send request
    STATUS_CHECK_RET(vs_snap_send_request(netif, mac, VS_PC_SERVICE_ID, VS_PC_PCMD, (uint8_t *)json, sz),
                     "Cannot send request");

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_pc_response_processor(const vs_mac_addr_t *dev_mac,
                       vs_snap_element_t element_id,
                       bool is_ack,
                       const uint8_t *response,
                       const uint16_t response_sz) {

    vs_status_e res = is_ack ? VS_CODE_OK : VS_CODE_ERR_SNAP_UNKNOWN;
    uint16_t str_sz;
    const char *json = (const char *)response;

    // Check input parameters
    CHECK_NOT_ZERO_RET(dev_mac, VS_CODE_ERR_INCORRECT_ARGUMENT);
    CHECK_NOT_ZERO_RET(response, VS_CODE_ERR_INCORRECT_ARGUMENT);
    CHECK_NOT_ZERO_RET(response_sz <= PC_JSON_SZ_MAX, VS_CODE_ERR_INCORRECT_ARGUMENT);
    str_sz = strnlen(json, PC_JSON_SZ_MAX);
    ++str_sz;
    CHECK_NOT_ZERO_RET(str_sz <= PC_JSON_SZ_MAX, VS_CODE_ERR_INCORRECT_ARGUMENT);

    if (_impl.device_state_update) {
        _impl.device_state_update(res, dev_mac, json);
    }

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_pc_client_response_processor(const struct vs_netif_t *netif,
                              const vs_ethernet_header_t *eth_header,
                              vs_snap_element_t element_id,
                              bool is_ack,
                              const uint8_t *response,
                              const uint16_t response_sz) {
    (void)netif;

    // Check input parameters
    CHECK_NOT_ZERO_RET(eth_header, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(response, VS_CODE_ERR_ZERO_ARGUMENT);

    switch (element_id) {

    case VS_PC_PCMD:
    case VS_PC_GPST:
        return _pc_response_processor(&eth_header->src, element_id, is_ack, response, response_sz);

    default:
        VS_LOG_ERROR("Unsupported PC command");
        VS_IOT_ASSERT(false);
        return VS_CODE_COMMAND_NO_RESPONSE;
    }
}

//-----------------------------------------------------------------------------
static vs_status_e
_ipst_request_processor(const vs_mac_addr_t *dev_mac,
                        const uint8_t *request,
                        const uint16_t request_sz,
                        uint8_t *response,
                        const uint16_t response_buf_sz,
                        uint16_t *response_sz) {

    *response_sz = 0;
    return _pc_response_processor(dev_mac, VS_PC_GPST, true, request, request_sz);
}

//-----------------------------------------------------------------------------
static vs_status_e
_pc_client_request_processor(const struct vs_netif_t *netif,
                             const vs_ethernet_header_t *eth_header,
                             vs_snap_element_t element_id,
                             const uint8_t *request,
                             const uint16_t request_sz,
                             uint8_t *response,
                             const uint16_t response_buf_sz,
                             uint16_t *response_sz) {
    (void)netif;

    // Check input parameters
    CHECK_NOT_ZERO_RET(eth_header, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(request, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(response, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(response_sz, VS_CODE_ERR_ZERO_ARGUMENT);

    *response_sz = 0;

    switch (element_id) {

    case VS_PC_IPST:
        return _ipst_request_processor(&eth_header->src, request, request_sz, response, response_buf_sz, response_sz);

    default:
        VS_LOG_ERROR("Unsupported PC command");
        return VS_CODE_COMMAND_NO_RESPONSE;
    }
}

//-----------------------------------------------------------------------------
const vs_snap_service_t *
vs_snap_user_client(vs_snap_user_client_service_t impl) {
    _pc_client.user_data = 0;
    _pc_client.id = VS_PC_SERVICE_ID;
    _pc_client.request_process = _pc_client_request_processor;
    _pc_client.response_process = _pc_client_response_processor;
    _pc_client.periodical_process = NULL;

    // Save callbacks
    VS_IOT_MEMCPY(&_impl, &impl, sizeof(impl));

    return &_pc_client;
}

//-----------------------------------------------------------------------------

#endif // PC_CLIENT
