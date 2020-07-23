//
//                                  _____   _______
//                                 |_   _| |__   __|
//                                   | |  ___ | |
//                                   | | / _ \| |
//                                  _| || (_) | |
//                                 |_____\___/|_|
//
//    _  ________ ______ _____    _____ _______    _____ _____ __  __ _____  _      ______
//   | |/ /  ____|  ____|  __ \  |_   _|__   __|  / ____|_   _|  \/  |  __ \| |    |  ____|
//   | ' /| |__  | |__  | |__) |   | |    | |    | (___   | | | \  / | |__) | |    | |__
//   |  < |  __| |  __| |  ___/    | |    | |     \___ \  | | | |\/| |  ___/| |    |  __|
//   | . \| |____| |____| |       _| |_   | |     ____) |_| |_| |  | | |    | |____| |____
//   |_|\_\______|______|_|      |_____|  |_|    |_____/|_____|_|  |_|_|    |______|______|
//
//
//
//   23 July 2020
//   Lead Maintainer: Roman Kutashenko <kutashenko@gmail.com>

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

// Types
typedef void (*vs_reboot_request_cb_t)(void);

// Structures
typedef struct {
    vs_reboot_request_cb_t reboot_request_cb;
} vs_iotkit_events_t;

// Functions
vs_status_e
iotkit_init(vs_device_manufacture_id_t manufacture_id,
                   vs_device_type_t device_type,
                   vs_device_serial_t serial,
                   uint32_t device_roles,
                   vs_secmodule_impl_t *secmodule_impl,
                   vs_storage_op_ctx_t *tl_storage_impl,
                   vs_netif_t *netif_impl[],
                   vs_snap_cfg_server_service_t cfg_server_cb,
                   vs_netif_process_cb_t packet_preprocessor_cb,
                   vs_iotkit_events_t iotkit_events);

vs_status_e
iotkit_deinit(void);

#endif // PROVISION_IOTKIT_INIT_H
