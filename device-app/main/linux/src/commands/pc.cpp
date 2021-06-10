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
#include <filesystem>

#include "common/helpers/timer.h"
#include "common/helpers/command.h"
#include "common/helpers/settings.h"

#include "commands/pc.h"

#include <common/protocols/snap/pc/pc-private.h>

#include <nlohmann/json.hpp>

static KSTimer _processingDelayer;
static const auto kDelayMs = std::chrono::milliseconds(200);

//-----------------------------------------------------------------------------
vs_status_e
ks_snap_pc_get_info_cb(const vs_netif_t *netif, char *state, const uint16_t state_buf_sz, uint16_t *state_sz) {
    CHECK_NOT_ZERO_RET(state, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(state_sz, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(state_buf_sz, VS_CODE_ERR_ZERO_ARGUMENT);

    nlohmann::json stateJson;
    stateJson["command"] = "info";
    stateJson["inet"] = false;
    stateJson["wifi"] = "sta";
    stateJson["wifi_ipv4"] = "10.220.0.7";
    stateJson["eth_ipv4"] = "192.168.0.20";

    auto jsonStr = stateJson.dump();

    if (jsonStr.size() > state_buf_sz) {
        return VS_CODE_ERR_TOO_SMALL_BUFFER;
    }
    strcpy(state, jsonStr.c_str());
    *state_sz = strlen(state) + 1;

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_status_e
ks_snap_pc_command_cb(const vs_netif_t *netif, vs_mac_addr_t sender_mac, const char *json) {
    CHECK_NOT_ZERO_RET(json, VS_CODE_ERR_NULLPTR_ARGUMENT);
    uint16_t json_sz = strnlen(json, PC_JSON_SZ_MAX);
    CHECK_RET(json_sz < PC_JSON_SZ_MAX, VS_CODE_ERR_INCORRECT_ARGUMENT, "Command request too long");

    VS_LOG_DEBUG("New command: %s", json);

    bool res = false;

    try {
        auto jsonObj = nlohmann::json::parse(json);
        auto command = jsonObj["command"].get<std::string>();
        CHECK_RET(command == "script", VS_CODE_ERR_INCORRECT_ARGUMENT, "Wrong command");

        auto script = jsonObj["script"].get<std::string>();
        auto params = jsonObj["params"];

        CHECK_RET(!script.empty(), VS_CODE_ERR_INCORRECT_ARGUMENT, "Script field is empty");

        // TODO: Prevent SHELL injections
        // check for a certain file
        // check for absence of &, &&, |, || etc
        // verify script signature

        auto commandStr = std::string(ks_settings_scripts_dir()) + "/" + script;

        for (const std::string &param : params) {
            commandStr += " \"" + param + "\"";
        }

        res = _processingDelayer.add(kDelayMs, [netif, sender_mac, commandStr]() -> void {
            Command cmd;
            cmd.Command = "bash -c '" + commandStr + "'";
            cmd.execute();

            // TODO: Remove it
            std::cout << cmd.StdOut;
            std::cerr << cmd.StdErr;
            std::cout << "Exit Status: " << cmd.ExitStatus << "\n";
            // ~TODO: Remove it

            bool is_ok = 0 == cmd.ExitStatus;

            // Prepare information about current state
            char state[PC_JSON_SZ_MAX];
            memset(state, 0, PC_JSON_SZ_MAX);
            uint16_t state_sz;

            if (VS_CODE_OK != ks_snap_pc_get_info_cb(netif, state, PC_JSON_SZ_MAX, &state_sz)) {
                VS_LOG_WARNING("Cannot get PC state");
                is_ok = false;
            }

            // Send response after complete processing
            if (VS_CODE_OK != vs_snap_send_response(netif,
                                                    &sender_mac,
                                                    0, // TODO: Fill transaction ID
                                                    VS_PC_SERVICE_ID,
                                                    VS_PC_PCMD,
                                                    is_ok,
                                                    reinterpret_cast<uint8_t *>(&state),
                                                    sizeof(state))) {
                VS_LOG_WARNING("Cannot initialize RPi.");
            }
        });
    } catch (...) {
        VS_LOG_WARNING("Command cannot be processed");
    }

    if (!res) {
        VS_LOG_WARNING("PC command processor is busy");
    }

    return VS_CODE_COMMAND_NO_RESPONSE;
}

//-----------------------------------------------------------------------------
