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

#ifndef YIOT_SNAP_SERVICES_LAMP_CLIENT_H
#define YIOT_SNAP_SERVICES_LAMP_CLIENT_H

#if LAMP_CLIENT

#include <virgil/iot/protocols/snap/snap-structs.h>
#include <common/protocols/snap/lamp/lamp-structs.h>

#ifdef __cplusplus
namespace VirgilIoTKit {
extern "C" {
#endif

typedef vs_status_e (*vs_snap_lamp_set_state_cb_t)(vs_snap_lamp_state_t *);
typedef vs_status_e (*vs_snap_lamp_get_state_cb_t)(void);

typedef vs_status_e (*vs_snap_lamp_state_notif_cb_t)(vs_status_e res,
                                                     const vs_mac_addr_t *mac,
                                                     const vs_snap_lamp_state_t *data);

/** LAMP client implementations
 *
 * \note Any callback can be NULL. In this case, there will be no actions with requests.
 *
 */
typedef struct {
    vs_snap_lamp_state_notif_cb_t device_state_update; /**< Current state notification */
} vs_snap_lamp_client_service_t;

/** Get lamp state
 *
 * This call can be broadcast
 *
 * \param[in] netif #vs_netif_t SNAP service descriptor. If NULL, default one will be used.
 * \param[in] mac #vs_mac_addr_t MAC address.
 *
 * \return #VS_CODE_OK in case of success or error code.
 */
vs_status_e
vs_snap_lamp_get_state(const vs_netif_t *netif, const vs_mac_addr_t *mac);

/** Set state of lamp
 *
 * \param[in] netif #vs_netif_t SNAP service descriptor. If NULL, default one will be used.
 * \param[in] mac #vs_mac_addr_t MAC address.
 * \param[in] required state of lamp #vs_snap_lamp_state_t.
 *
 * \return #VS_CODE_OK in case of success or error code.
 */
vs_status_e
vs_snap_lamp_set_state(const vs_netif_t *netif, const vs_mac_addr_t *mac, const vs_snap_lamp_state_t *state);

/** LAMP Client SNAP Service implementation
 *
 * This call returns LAMP client implementation. It must be called before any LAMP call.
 *
 * \param[in] impl Snap LAMP Client functions implementation.
 *
 * \return #vs_snap_service_t SNAP service description. Use this pointer to call #vs_snap_register_service.
 */
const vs_snap_service_t *
vs_snap_lamp_client(vs_snap_lamp_client_service_t impl);

#ifdef __cplusplus
} // extern "C"
} // namespace VirgilIoTKit
#endif

#endif // LAMP_CLIENT
#endif // YIOT_SNAP_SERVICES_LAMP_CLIENT_H
