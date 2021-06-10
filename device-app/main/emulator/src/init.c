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

#include <virgil/iot/protocols/snap/info/info-server.h>
#include <virgil/iot/protocols/snap/cfg/cfg-server.h>
#include <virgil/iot/protocols/snap/prvs/prvs-server.h>
#include <virgil/iot/protocols/snap/scrt/scrt-server.h>
#include <common/protocols/snap/pc/pc-server.h>

#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/provision/provision.h>
#include <virgil/iot/session/session.h>
#include <virgil/iot/secbox/secbox.h>

#include "init.h"

#include "common/iotkit-impl/netif/netif-websock.h"

static void
_file_ver_info_cb(vs_file_version_t ver);

static vs_provision_events_t _provision_event = {_file_ver_info_cb};

// Dev name
static vs_storage_element_id_t _device_name_storage_id = {0};
static const char *_defaultDevName = "My new RPi";

#define BLE_DEVICE_NAME_LIMIT (16)

//-----------------------------------------------------------------------------
static bool
need_enc_cb(vs_snap_service_id_t service_id, vs_snap_element_t element_id) {
    if (VS_PC_SERVICE_ID == service_id && VS_PC_PCMD == element_id) {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
static vs_status_e
init_name_storage_id(void) {
    if (!_device_name_storage_id[0]) {
        VS_IOT_MEMSET(_device_name_storage_id, 0, sizeof(_device_name_storage_id));
        VS_IOT_STRCPY((char *)_device_name_storage_id, "dev_name");
    }
    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
name_change_cb(void) {
    vs_status_e res;
    CHECK_NOT_ZERO_RET(vs_snap_device_name(), VS_CODE_ERR_INCORRECT_ARGUMENT);
    res = vs_secbox_save(VS_SECBOX_SIGNED_AND_ENCRYPTED,
                         _device_name_storage_id,
                         (const uint8_t *)vs_snap_device_name(),
                         strnlen(vs_snap_device_name(), DEVICE_NAME_SZ_MAX));
    return res;
}

//-----------------------------------------------------------------------------
static vs_status_e
init_dev_name(void) {
    vs_status_e ret_code;
    uint8_t name_buf[DEVICE_NAME_SZ_MAX];
    size_t name_sz;
    const char *name;

    VS_IOT_MEMSET(name_buf, 0, DEVICE_NAME_SZ_MAX);
    init_name_storage_id();

    if (VS_CODE_OK == vs_secbox_load(_device_name_storage_id, name_buf, DEVICE_NAME_SZ_MAX, &name_sz)) {
        if (name_sz > BLE_DEVICE_NAME_LIMIT) {
            name_sz = BLE_DEVICE_NAME_LIMIT;
        }
        name_buf[name_sz] = 0x00;
        name = (char *)name_buf;
    } else {
        name = _defaultDevName;
    }

    STATUS_CHECK_RET(vs_snap_init_device_name(name, false), "Unable to set device name");
    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_status_e
ks_iotkit_init(vs_device_manufacture_id_t manufacture_id,
               vs_device_type_t device_type,
               vs_device_serial_t serial,
               uint32_t device_roles,
               vs_netif_t *netif_impl[],
               vs_snap_cfg_server_service_t cfg_server_cb,
               vs_snap_pc_server_service_t pc_server_cb,
               vs_netif_process_cb_t packet_preprocessor_cb,
               vs_secmodule_impl_t *secmodule_impl,
               vs_storage_op_ctx_t *tl_storage_impl) {
    vs_status_e res = VS_CODE_ERR_INIT_SNAP;
    vs_status_e ret_code;
    uint8_t i = 1;
    vs_snap_scrt_server_service_t scrt_server_cb = {0};
    VS_IOT_ASSERT(netif_impl);
    VS_IOT_ASSERT(netif_impl[0]);

    //
    // ---------- Initialize Virgil SDK modules ----------
    //

    // Provision module
    VS_IOT_ASSERT(secmodule_impl);
    VS_IOT_ASSERT(tl_storage_impl);
    ret_code = vs_provision_init(tl_storage_impl, secmodule_impl, _provision_event);
    if (VS_CODE_OK != ret_code && VS_CODE_ERR_PROVISION_NOT_READY != ret_code) {
        VS_LOG_ERROR("Cannot initialize Provision module");
        goto terminate;
    }

    // SNAP module

    STATUS_CHECK(init_dev_name(), "Cannot init device name");
    STATUS_CHECK(vs_snap_init(netif_impl[0],
                              packet_preprocessor_cb,
                              need_enc_cb,
                              name_change_cb,
                              manufacture_id,
                              device_type,
                              serial,
                              device_roles),
                 "Unable to initialize SNAP module");


    while (netif_impl[i] != NULL) {
        STATUS_CHECK_RET(vs_snap_netif_add(netif_impl[i]), "Unable to add netif to a SNAP module");
        ++i;
    }

    // Get main MAC address
    vs_mac_addr_t default_mac;
    vs_snap_mac_addr(netif_impl[0], &default_mac);

    // Security Session module
    vs_session_init(secmodule_impl, default_mac.bytes);

    //
    // ---------- Register SNAP services ----------
    //

    //  PRVS service
    const vs_snap_service_t *snap_prvs_server;
    snap_prvs_server = vs_snap_prvs_server(secmodule_impl);
    STATUS_CHECK(vs_snap_register_service(snap_prvs_server), "Cannot register PRVS service");

    //  SCRT server service
    const vs_snap_service_t *snap_scrt_server;
    snap_scrt_server = vs_snap_scrt_server(secmodule_impl, scrt_server_cb);
    STATUS_CHECK(vs_snap_register_service(snap_scrt_server), "Cannot register SCRT server service");

    //  INFO server service
    const vs_snap_service_t *snap_info_server;
    snap_info_server = vs_snap_info_server(NULL);
    STATUS_CHECK(vs_snap_register_service(snap_info_server), "Cannot register INFO server service");

    // CFG server service
    const vs_snap_service_t *snap_cfg_server;
    snap_cfg_server = vs_snap_cfg_server(cfg_server_cb);
    STATUS_CHECK(vs_snap_register_service(snap_cfg_server), "Cannot register CFG server service");

    // PC server service
    const vs_snap_service_t *snap_pc_server;
    snap_pc_server = vs_snap_pc_server(pc_server_cb);
    STATUS_CHECK(vs_snap_register_service(snap_pc_server), "Cannot register PC server service");

    res = VS_CODE_OK;

terminate:

    return res;
}

//-----------------------------------------------------------------------------
vs_status_e
ks_iotkit_deinit(void) {

    // Deinit Virgil SDK modules
    vs_snap_deinit();

    // Deinit provision
    vs_provision_deinit();

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static void
_file_ver_info_cb(vs_file_version_t ver) {
    vs_snap_info_set_tl_ver(ver);
}

//-----------------------------------------------------------------------------
