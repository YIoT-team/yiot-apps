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

#ifndef YIOT_SNAP_SERVICES_PC_STRUCTS_H
#define YIOT_SNAP_SERVICES_PC_STRUCTS_H

#include <stdint.h>

#ifdef __cplusplus
namespace VirgilIoTKit {
extern "C" {
#endif

#define USER_NAME_SZ_MAX 64
#define USER_PASS_SZ_MAX 64

typedef struct __attribute__((__packed__)) {
    uint8_t user[USER_NAME_SZ_MAX];
    uint8_t pass[USER_PASS_SZ_MAX];
    uint32_t ipv4;
} vs_snap_pc_init_t;

typedef enum {
    KS_PC_WIFI_UNKNOWN = 0,
    KS_PC_WIFI_STA,
    KS_PC_WIFI_AP
} vs_snap_pc_wifi_mode_e;

typedef struct __attribute__((__packed__)) {
    uint8_t internet_present;
    uint8_t wifi_mode;
    uint32_t wifi_ipv4;
    uint32_t ethernet_ipv4;
} vs_snap_pc_state_t;

#ifdef __cplusplus
} // extern "C"
} // namespace VirgilIoTKit
#endif

#endif // YIOT_SNAP_SERVICES_PC_STRUCTS_H
