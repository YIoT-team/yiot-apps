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

#ifndef YIOT_SNAP_SERVICES_PC_PRIVATE_H
#define YIOT_SNAP_SERVICES_PC_PRIVATE_H

#include <common/protocols/snap/pc/pc-server.h>
#include <common/protocols/snap/pc/pc-structs.h>
#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/status_code/status_code.h>
#include <virgil/iot/protocols/snap/snap-structs.h>

// mute "error: multi-character character constant" message
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmultichar"
typedef enum { VS_PC_SERVICE_ID = HTONL_IN_COMPILE_TIME('_PC_') } vs_pc_t;

typedef enum {
    VS_PC_INPC = HTONL_IN_COMPILE_TIME('INPC'), /* INit PC */
    VS_PC_GPST = HTONL_IN_COMPILE_TIME('GPST'), /* Get Pc STate */
} vs_snap_pc_element_e;
#pragma GCC diagnostic pop

#endif // YIOT_SNAP_SERVICES_PC_PRIVATE_H
