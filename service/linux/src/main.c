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
//   21 July 2020
//   Lead Maintainer: Roman Kutashenko <kutashenko@gmail.com>

#include <unistd.h>
#include <virgil/iot/logger/logger.h>
#include <virgil/iot/macros/macros.h>
#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/vs-soft-secmodule/vs-soft-secmodule.h>
#include <virgil/iot/protocols/snap/info/info-server.h>

#include "helpers/app-helpers.h"

#include "sdk-impl/firmware/firmware-nix-impl.h"
#include "sdk-impl/netif/netif-udp-broadcast.h"
#include "sdk-impl/netif/packets-queue.h"

#include "iotkit-impl/init.h"

#if SECURE_PROVISION
#include <trust_list-config.h>
#include <update-config.h>
#include "helpers/app-storage.h"
#include "helpers/file-cache.h"
#endif

/******************************************************************************/
int
main(int argc, char *argv[]) {
    int res = -1;

    // Implementation variables
    vs_secmodule_impl_t *secmodule_impl = NULL;
    vs_netif_t *netifs_impl[2] = {NULL, NULL};
    vs_storage_op_ctx_t tl_storage_impl;
    vs_storage_op_ctx_t slots_storage_impl;
    vs_storage_op_ctx_t fw_storage_impl;
    vs_snap_cfg_server_service_t cfg_server_cb = {NULL, NULL, NULL, NULL};

    // Device parameters

    // TODO: get from config file
    vs_device_manufacture_id_t manufacture_id = {0};
    vs_device_type_t device_type = {0};

    //  TODO: function to get serial
    vs_device_serial_t serial = {0};

    // Initialize Logger module
    vs_logger_init(VS_LOGLEV_DEBUG);

    // Print title
    vs_app_print_title("BLE provision service", argv[0], "", "");

    //
    // ---------- Create implementations ----------
    //

    // Network interface
    vs_packets_queue_init(vs_snap_default_processor);
    vs_mac_addr_t mac_addr;
    netifs_impl[0] = vs_hal_netif_udp_bcast(mac_addr);

#if SECURE_PROVISION
    // TrustList storage
    STATUS_CHECK(vs_app_storage_init_impl(&tl_storage_impl, vs_app_trustlist_dir(), VS_TL_STORAGE_MAX_PART_SIZE),
                 "Cannot create TrustList storage");

    // Slots storage
    STATUS_CHECK(vs_app_storage_init_impl(&slots_storage_impl, vs_app_slots_dir(), VS_SLOTS_STORAGE_MAX_SIZE),
                 "Cannot create TrustList storage");

    // Soft Security Module
    secmodule_impl = vs_soft_secmodule_impl(&slots_storage_impl);
#endif // SECURE_PROVISION

    //
    // ---------- Initialize IoTKit internals ----------
    //

    // Initialize IoTKit
    STATUS_CHECK(iotkit_init(manufacture_id,
                             device_type,
                             serial,
                             VS_SNAP_DEV_THING,
                             netifs_impl,
                             cfg_server_cb,
                             vs_packets_queue_add
#if SECURE_PROVISION
                             ,
                             secmodule_impl,
                             &tl_storage_impl
#endif
                             ),
                 "Cannot initialize IoTKit");

    //
    // ---------- Application work ----------
    //

    // Send broadcast notification about self start
    vs_snap_info_start_notification(vs_snap_netif_routing());

    // Sleep until CTRL_C
    vs_app_sleep_until_stop();

    //
    // ---------- Terminate application ----------
    //

    res = 0;

terminate:

    VS_LOG_INFO("\n\n\n");
    VS_LOG_INFO("Terminating application ...");

    // De-initialize IoTKit internals
    iotkit_deinit();

#if SECURE_PROVISION
    // Deinit Soft Security Module
    vs_soft_secmodule_deinit();
#endif

    vs_packets_queue_deinit();

    return res;
}

/******************************************************************************/
