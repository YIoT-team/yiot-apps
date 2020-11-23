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

#ifndef PROVISION_IOTKIT_INIT_H
#define PROVISION_IOTKIT_INIT_H

#include <stdarg.h>
#include <string.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib-config.h>

#include <virgil/iot/status_code/status_code.h>
#include <virgil/iot/provision/provision-structs.h>
#include <virgil/iot/secmodule/secmodule.h>
#include <virgil/iot/storage_hal/storage_hal.h>
#include <virgil/iot/protocols/snap/snap-structs.h>
#include <virgil/iot/protocols/snap/cfg/cfg-server.h>

// Functions
vs_status_e
ks_iotkit_init(vs_device_manufacture_id_t manufacture_id,
               vs_device_type_t device_type,
               vs_device_serial_t serial,
               uint32_t device_roles,
               vs_netif_t *netif_impl[],
               vs_snap_cfg_server_service_t cfg_server_cb,
               vs_netif_process_cb_t packet_preprocessor_cb
#if SECURE_PROVISION
               ,
               vs_secmodule_impl_t *secmodule_impl,
               vs_storage_op_ctx_t *tl_storage_impl
#endif
);

vs_status_e
ks_iotkit_deinit(void);

#endif // PROVISION_IOTKIT_INIT_H
