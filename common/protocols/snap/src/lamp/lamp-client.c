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

#if LAMP_CLIENT

#include <common/protocols/snap/lamp/lamp-client.h>
#include <common/protocols/snap/lamp/lamp-private.h>
#include <virgil/iot/protocols/snap/generated/snap_cvt.h>
#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/status_code/status_code.h>
#include <virgil/iot/macros/macros.h>
#include <virgil/iot/logger/logger.h>
#include <stdlib-config.h>
#include <global-hal.h>

// External functions for access to upper level implementations
static vs_snap_service_t _lamp_client = {0};
static vs_snap_lamp_client_service_t _impl = {0};

//-----------------------------------------------------------------------------
vs_status_e
vs_snap_lamp_get_state(const vs_netif_t *netif, const vs_mac_addr_t *mac) {
    const vs_mac_addr_t *dst_mac;
    vs_status_e ret_code;

    // Set destination mac
    dst_mac = mac ? mac : vs_snap_broadcast_mac();

    // Send request
    STATUS_CHECK_RET(vs_snap_send_request(netif, dst_mac, VS_LAMP_SERVICE_ID, VS_LAMP_GLST, NULL, 0),
                     "Cannot send request");

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_status_e
vs_snap_lamp_set_state(const vs_netif_t *netif, const vs_mac_addr_t *mac, const vs_snap_lamp_state_t *state) {
    const vs_mac_addr_t *dst_mac;
    vs_status_e ret_code;

    // Check input parameters
    CHECK_NOT_ZERO_RET(state, VS_CODE_ERR_INCORRECT_ARGUMENT);

    // Set destination mac
    dst_mac = mac ? mac : vs_snap_broadcast_mac();

    // TODO: Normalize structure

    // Send request
    STATUS_CHECK_RET(
            vs_snap_send_request(netif, dst_mac, VS_LAMP_SERVICE_ID, VS_LAMP_SLST, (uint8_t *)state, sizeof(*state)),
            "Cannot send request");

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_lamp_response_processor(vs_snap_element_t element_id,
                         bool is_ack,
                         const uint8_t *response,
                         const uint16_t response_sz) {

    vs_status_e res = is_ack ? VS_CODE_OK : VS_CODE_ERR_SNAP_UNKNOWN;
    vs_snap_lamp_state_t *state = NULL;

    if (is_ack) {
        state = (vs_snap_lamp_state_t *)response;
    }

    // TODO: Normalize structure

    if (_impl.device_state_update) {
        // TODO: Use real mac addr
        _impl.device_state_update(res, vs_snap_broadcast_mac(), state);
    }

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_lamp_client_response_processor(const struct vs_netif_t *netif,
                                const vs_ethernet_header_t *eth_header,
                                vs_snap_element_t element_id,
                                bool is_ack,
                                const uint8_t *response,
                                const uint16_t response_sz) {
    (void)netif;

    switch (element_id) {

    case VS_LAMP_GLST:
    case VS_LAMP_SLST:
        return _lamp_response_processor(element_id, is_ack, response, response_sz);

    default:
        VS_LOG_ERROR("Unsupported LAMP command");
        VS_IOT_ASSERT(false);
        return VS_CODE_COMMAND_NO_RESPONSE;
    }
}

//-----------------------------------------------------------------------------
const vs_snap_service_t *
vs_snap_lamp_client(vs_snap_lamp_client_service_t impl) {

    _lamp_client.user_data = 0;
    _lamp_client.id = VS_LAMP_SERVICE_ID;
    _lamp_client.request_process = NULL;
    _lamp_client.response_process = _lamp_client_response_processor;
    _lamp_client.periodical_process = NULL;

    // Save callbacks
    VS_IOT_MEMCPY(&_impl, &impl, sizeof(impl));

    return &_lamp_client;
}

//-----------------------------------------------------------------------------

#endif // LAMP_CLIENT
