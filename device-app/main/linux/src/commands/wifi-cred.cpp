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

#include "commands/wifi-cred.h"

#include <virgil/iot/protocols/snap/info/info-server.h>
#include <virgil/iot/protocols/snap/cfg/cfg-private.h>
#include <common/protocols/snap/pc/pc-server.h>

static KSTimer _processingDelayer;
static const auto kDelayMs = std::chrono::milliseconds(200);

//-----------------------------------------------------------------------------
vs_status_e
ks_snap_cfg_wifi_cb(const vs_netif_t *netif,
                    vs_mac_addr_t sender_mac,
                    const vs_cfg_wifi_configuration_t *configuration) {
    CHECK_NOT_ZERO_RET(configuration, VS_CODE_ERR_NULLPTR_ARGUMENT);

    VS_LOG_DEBUG("ssid: %s", configuration->ssid);
    VS_LOG_DEBUG("pass: *******");

    auto ssid = std::string(reinterpret_cast<const char *>(configuration->ssid));
    auto pass = std::string(reinterpret_cast<const char *>(configuration->pass));
    auto command = std::string(ks_settings_scripts_dir()) + "/yiot-enable-wifi-sta.sh " + ssid + " " + pass;

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
        bool success = VS_CODE_OK == vs_snap_send_response(netif,
                                                           &sender_mac,
                                                           0, // TODO: Fill transaction ID
                                                           VS_CFG_SERVICE_ID,
                                                           VS_CFG_WIFI,
                                                           0 == cmd.ExitStatus,
                                                           NULL,
                                                           0);
        if (!success) {
            VS_LOG_WARNING("Cannot set WiFi credentials.");
        }

        vs_snap_info_set_need_cred(!success);

        // TODO: Move it IoTKit
        vs_provision_update();

        // TODO: Use callback from IoTKit
        const vs_netif_t *n = vs_snap_default_netif();
        vs_snap_pc_start_notification(n);
    });

    if (!res) {
        VS_LOG_WARNING("wifi-cred is busy");
    }

    return VS_CODE_COMMAND_NO_RESPONSE;
}

//-----------------------------------------------------------------------------
