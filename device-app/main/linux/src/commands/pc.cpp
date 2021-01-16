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

#include <arpa/inet.h>

#include <iostream>

#include "helpers/timer.h"
#include "helpers/command.h"
#include "helpers/settings.h"

#include "commands/pc.h"

#include <common/protocols/snap/pc/pc-private.h>

static KSTimer _processingDelayer;
static const auto kDelayMs = std::chrono::milliseconds(200);

//-----------------------------------------------------------------------------
vs_status_e
ks_snap_pc_get_info_cb(const vs_netif_t *netif, vs_mac_addr_t sender_mac, const char *state) {
    //    CHECK_NOT_ZERO_RET(state, VS_CODE_ERR_ZERO_ARGUMENT);
    //
    //    state->internet_present = 1;
    //    state->wifi_mode = KS_PC_WIFI_STA;
    //    state->wifi_ipv4 = 0x1234;
    //    state->ethernet_ipv4 = 0xABCD;

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_status_e
ks_snap_pc_init_ssh_cb(const vs_netif_t *netif, vs_mac_addr_t sender_mac, const char *init) {
    //    CHECK_NOT_ZERO_RET(init, VS_CODE_ERR_NULLPTR_ARGUMENT);
    //
    //    // Get string representation of IP address
    //    struct sockaddr_in sa;
    //    char ipv4[INET_ADDRSTRLEN];
    //    memset(&sa, 0, sizeof(sa));
    //    sa.sin_addr.s_addr = init->ipv4;
    //    sa.sin_family = AF_INET;
    //    if (NULL == inet_ntop(AF_INET, &(sa.sin_addr), ipv4, INET_ADDRSTRLEN)) {
    //        VS_LOG_WARNING("Cannot convert IPv4 address");
    //    }
    //
    //    VS_LOG_DEBUG("User: %s", init->user);
    //    VS_LOG_DEBUG("pass: *******");
    //    VS_LOG_DEBUG("IPv4: %s", ipv4);
    //
    //    auto user = std::string(reinterpret_cast<const char *>(init->user));
    //    auto pass = std::string(reinterpret_cast<const char *>(init->pass));
    //    auto command = std::string(ks_settings_scripts_dir()) + "/yiot-initialize-rpi.sh " + user + " " + pass + " " +
    //    ipv4;
    //
    //    bool res = _processingDelayer.add(kDelayMs, [netif, sender_mac, command]() -> void {
    //        Command cmd;
    //        cmd.Command = "bash -c \"" + command + "\"";
    //        cmd.execute();
    //
    //        // TODO: Remove it
    //        std::cout << cmd.StdOut;
    //        std::cerr << cmd.StdErr;
    //        std::cout << "Exit Status: " << cmd.ExitStatus << "\n";
    //        // ~TODO: Remove it
    //
    //        bool is_ok = 0 == cmd.ExitStatus;
    //
    //        // Prepare information about current state
    //        vs_snap_pc_state_t state;
    //        if (VS_CODE_OK != ks_snap_pc_get_info_cb(netif, sender_mac, &state)) {
    //            VS_LOG_WARNING("Cannot get PC status");
    //            is_ok = false;
    //        }
    //
    //        // Send response after complete processing
    //        if (VS_CODE_OK != vs_snap_send_response(netif,
    //                                                &sender_mac,
    //                                                0, // TODO: Fill transaction ID
    //                                                VS_PC_SERVICE_ID,
    //                                                VS_PC_ISSH,
    //                                                is_ok,
    //                                                reinterpret_cast<uint8_t *>(&state),
    //                                                sizeof(state))) {
    //            VS_LOG_WARNING("Cannot initialize RPi.");
    //        }
    //    });
    //
    //    return res ? VS_CODE_COMMAND_NO_RESPONSE : VS_CODE_ERR_QUEUE;
    return VS_CODE_ERR_NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
