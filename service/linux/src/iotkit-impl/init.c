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

#include <virgil/iot/high-level/high-level.h>
#include <virgil/iot/protocols/snap/info/info-server.h>
#include <virgil/iot/protocols/snap/prvs/prvs-server.h>
#include <virgil/iot/protocols/snap/fldt/fldt-server.h>
#include <virgil/iot/protocols/snap/cfg/cfg-server.h>
#include <virgil/iot/protocols/snap/fldt/fldt-client.h>
#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/provision/provision.h>

static void
_file_ver_info_cb(vs_file_version_t ver);

static vs_provision_events_t _provision_event = {_file_ver_info_cb};
static vs_iotkit_events_t _iotkit_events = {NULL};

/******************************************************************************/
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
                   vs_iotkit_events_t iotkit_events) {
    vs_status_e res = VS_CODE_ERR_INIT_SNAP;
    vs_status_e ret_code;
    uint8_t i = 1;
    VS_IOT_ASSERT(secmodule_impl);
    VS_IOT_ASSERT(tl_storage_impl);
    VS_IOT_ASSERT(netif_impl);
    VS_IOT_ASSERT(netif_impl[0]);

    // Save callbacks
    _iotkit_events = iotkit_events;

    //
    // ---------- Initialize Virgil SDK modules ----------
    //

    // Provision module
    ret_code = vs_provision_init(tl_storage_impl, secmodule_impl, _provision_event);
    if (VS_CODE_OK != ret_code && VS_CODE_ERR_NOINIT != ret_code) {
        VS_LOG_ERROR("Cannot initialize Provision module");
        goto terminate;

    // SNAP module
    STATUS_CHECK(vs_snap_init(netif_impl[0], packet_preprocessor_cb, manufacture_id, device_type, serial, device_roles),
                 "Unable to initialize SNAP module");


    while (netif_impl[i] != NULL) {
        STATUS_CHECK_RET(vs_snap_netif_add(netif_impl[i]), "Unable to add netif to a SNAP module");
        ++i;
    }

    //
    // ---------- Register SNAP services ----------
    //

    //  INFO server service
    const vs_snap_service_t *snap_info_server;
    snap_info_server = vs_snap_info_server(NULL);
    STATUS_CHECK(vs_snap_register_service(snap_info_server), "Cannot register INFO server service");

    const vs_snap_service_t *snap_cfg_server;
    snap_cfg_server = vs_snap_cfg_server(cfg_server_cb);
    STATUS_CHECK(vs_snap_register_service(snap_cfg_server), "Cannot register CFG server service");

    //  PRVS service
    const vs_snap_service_t *snap_prvs_server;
    snap_prvs_server = vs_snap_prvs_server(secmodule_impl);
    STATUS_CHECK(vs_snap_register_service(snap_prvs_server), "Cannot register PRVS service");

    res = VS_CODE_OK;

terminate:

    return res;
}

/******************************************************************************/
vs_status_e
iotkit_deinit(void) {

    // Deinit Virgil SDK modules
    vs_snap_deinit();

    // Deinit provision
    vs_provision_deinit();

    return VS_CODE_OK;
}

/******************************************************************************/
static void
_file_ver_info_cb(vs_file_version_t ver) {
    vs_snap_info_set_tl_ver(ver);
}

/******************************************************************************/
