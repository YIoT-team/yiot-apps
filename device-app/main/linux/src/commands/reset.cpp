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

#include <iostream>

#include "helpers/timer.h"
#include "helpers/command.h"
#include "helpers/settings.h"

#include "commands/reset.h"

#include <virgil/iot/protocols/snap/cfg/cfg-private.h>

static KSTimer _processingDelayer;
static const auto kDelayMs = std::chrono::milliseconds(200);

//-----------------------------------------------------------------------------
vs_status_e
ks_snap_cfg_reset_cb(const vs_netif_t *netif, vs_mac_addr_t sender_mac) {
    auto command = std::string(ks_settings_scripts_dir()) + "/yiot-reset-rpi.sh";

    bool res = _processingDelayer.add(kDelayMs, [netif, sender_mac, command]() -> void {
        Command cmd;
        cmd.Command = "bash -c \"" + command + "\"";
        cmd.execute();

        // TODO: Remove it
        std::cout << cmd.StdOut;
        std::cerr << cmd.StdErr;
        std::cout << "Exit Status: " << cmd.ExitStatus << "\n";
        // ~TODO: Remove it

        // Send response after complete processing
        if (VS_CODE_OK != vs_snap_send_response(netif,
                                                &sender_mac,
                                                0, // TODO: Fill transaction ID
                                                VS_CFG_SERVICE_ID,
                                                VS_CFG_WIFI,
                                                0 == cmd.ExitStatus,
                                                NULL,
                                                0)) {
            VS_LOG_WARNING("Cannot set WiFi credentials.");
        }
    });

    if (!res) {
        VS_LOG_WARNING("reset processor is busy");
    }

    return VS_CODE_COMMAND_NO_RESPONSE;
}

//-----------------------------------------------------------------------------
