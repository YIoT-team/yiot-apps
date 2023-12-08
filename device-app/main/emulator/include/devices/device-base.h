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

#ifndef YIOT_TEST_DEVICE_BASE_H
#define YIOT_TEST_DEVICE_BASE_H

#include <virgil/iot/status_code/status_code.h>
#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/provision/provision-structs.h>
#include <common/protocols/snap/user/user-structs.h>

#ifdef __cplusplus
using namespace VirgilIoTKit;
extern "C" {
#endif

typedef void (*ks_func_manufacturer_t)(vs_device_manufacture_id_t manufacture_id);
typedef void (*ks_func_type_t)(vs_device_type_t device_type);
typedef void (*ks_func_serial_t)(vs_device_serial_t serial);
typedef vs_status_e (*ks_func_get_info_cb_t)(const vs_netif_t *netif,
                                             char *state,
                                             const uint16_t state_buf,
                                             uint16_t *state_sz);
typedef vs_status_e (*ks_func_command_cb_t)(const vs_netif_t *netif, vs_mac_addr_t sender_mac, const char *json);
typedef const char *(*ks_func_description_t)(void);
typedef const char *(*ks_func_default_name_t)(void);

typedef struct {
    ks_func_manufacturer_t get_manufacturer;
    ks_func_type_t get_type;
    ks_func_serial_t get_serial;
    ks_func_description_t get_description;
    ks_func_default_name_t get_default_name;
    ks_func_get_info_cb_t info_cb;
    ks_func_command_cb_t command_cb;
} ks_test_device_t;

#ifdef __cplusplus
}
#endif

#endif // YIOT_TEST_DEVICE_BASE_H
