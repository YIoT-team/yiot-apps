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

#include <virgil/iot/logger/logger.h>
#include <virgil/iot/macros/macros.h>
#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/protocols/snap/info/info-server.h>
#include <virgil/iot/protocols/snap/cfg/cfg-server.h>

#include "helpers/app-helpers.h"

#include "sdk-impl/netif/packets-queue.h"

#include "iotkit-impl/init.h"
#include "iotkit-impl/netif/netif-ble-linux.h"

#include "commands/device-info.h"
#include "commands/wifi-cred.h"

#if SECURE_PROVISION
#include <virgil/iot/vs-soft-secmodule/vs-soft-secmodule.h>
#include <trust_list-config.h>
#include <update-config.h>
#include "helpers/app-storage.h"
#include "helpers/file-cache.h"
#endif

static void
_print_title(void);

//-----------------------------------------------------------------------------
int
main(int argc, char *argv[]) {
    int res = -1;

    // Implementation variables
    vs_netif_t *netifs_impl[2] = {NULL, NULL};
    vs_snap_cfg_server_service_t cfg_server_cb = {ks_snap_cfg_wifi_cb, NULL, NULL, NULL};

#if SECURE_PROVISION
    vs_secmodule_impl_t *secmodule_impl = NULL;
    vs_storage_op_ctx_t tl_storage_impl;
    vs_storage_op_ctx_t slots_storage_impl;
    vs_storage_op_ctx_t fw_storage_impl;
#endif

    // Device parameters
    vs_device_manufacture_id_t manufacture_id;
    vs_device_type_t device_type;
    vs_device_serial_t serial;

    ks_devinfo_manufacturer(manufacture_id);
    ks_devinfo_device_type(device_type);
    ks_devinfo_device_serial(serial);

    // Initialize Logger module
    vs_logger_init(VS_LOGLEV_DEBUG);

    // Print title
    _print_title();

    //
    // ---------- Create implementations ----------
    //

    // Network interface
    vs_packets_queue_init(vs_snap_default_processor);
    netifs_impl[0] = ks_netif_ble();

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
    STATUS_CHECK(ks_iotkit_init(manufacture_id,
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
    vs_snap_info_start_notification(vs_snap_default_netif());

    //-----------------------
    vs_cfg_wifi_configuration_t c;
    strcpy(c.ssid, "SSID");
    strcpy(c.pass, "PASSWORD");
    ks_snap_cfg_wifi_cb(&c);
    //-----------------------

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
    ks_iotkit_deinit();

#if SECURE_PROVISION
    // Deinit Soft Security Module
    vs_soft_secmodule_deinit();
#endif

    // De-initialize SNAP packets queue
    vs_packets_queue_deinit();

    return res;
}

//-----------------------------------------------------------------------------
static void
_print_title(void) {
    vs_device_serial_t serial;
    char str_manufacturer[VS_DEVICE_MANUFACTURE_ID_SIZE + 1];
    char str_dev_type[VS_DEVICE_TYPE_SIZE + 1];
    char str_dev_serial[VS_DEVICE_SERIAL_SIZE * 2 + 1];
    vs_device_manufacture_id_t *manufacture = (vs_device_manufacture_id_t *)&str_manufacturer;
    vs_device_type_t *dev_type = (vs_device_type_t *)&str_dev_type;
    uint32_t in_out_len = VS_DEVICE_SERIAL_SIZE * 2 + 1;

    ks_devinfo_manufacturer(*manufacture);
    ks_devinfo_device_type(*dev_type);
    ks_devinfo_device_serial(serial);
    vs_app_data_to_hex(serial, VS_DEVICE_SERIAL_SIZE, (uint8_t *)str_dev_serial, &in_out_len);

    VS_LOG_INFO("\n\n");
    VS_LOG_INFO("------------- KEEP IT SIMPLE ---------------");
#if SECURE_PROVISION
    VS_LOG_INFO("      Secure WiFi provision service.");
#else
    VS_LOG_INFO("          WiFi provision service.");
#endif

    VS_LOG_INFO("Manufacture ID = \"%s\"", str_manufacturer);
    VS_LOG_INFO("Device type    = \"%s\"", str_dev_type);
    VS_LOG_INFO("Device serial  = \"%s\"", str_dev_type);
    VS_LOG_INFO("--------------------------------------------\n");
}

//-----------------------------------------------------------------------------
