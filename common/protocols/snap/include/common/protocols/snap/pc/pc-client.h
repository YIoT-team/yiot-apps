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

#ifndef YIOT_SNAP_SERVICES_PC_CLIENT_H
#define YIOT_SNAP_SERVICES_PC_CLIENT_H

#if PC_CLIENT

#include <virgil/iot/protocols/snap/snap-structs.h>
#include <common/protocols/snap/pc/pc-structs.h>

#ifdef __cplusplus
namespace VirgilIoTKit {
extern "C" {
#endif

typedef vs_status_e (*vs_snap_pc_state_notif_cb_t)(vs_status_e res, const vs_mac_addr_t *mac, const char *json);

typedef struct {
    vs_snap_pc_state_notif_cb_t device_state_update; /**< Current state notification */
} vs_snap_pc_client_service_t;

vs_status_e
vs_snap_pc_get_state(const vs_netif_t *netif, const vs_mac_addr_t *mac);

vs_status_e
vs_snap_pc_command(const vs_netif_t *netif, const vs_mac_addr_t *mac, const char *json);

const vs_snap_service_t *
vs_snap_pc_client(vs_snap_pc_client_service_t impl);

#ifdef __cplusplus
} // extern "C"
} // namespace VirgilIoTKit
#endif

#endif // PC_CLIENT
#endif // YIOT_SNAP_SERVICES_PC_CLIENT_H
