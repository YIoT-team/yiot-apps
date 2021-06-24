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
#include "devices/test-lamp.h"
#include <common/protocols/snap/user/user-private.h>
#include <nlohmann/json.hpp>

static ks_test_device_t _lamp = {0};

// clang-format off
char offStateImage[] =
"                              **#######**                             \n"
"                          *######*****######*                         \n"
"                       *####*             *####*                      \n"
"                     *###*                   *###*                    \n"
"                   ###                           ###                  \n"
"                  ###                             ###                 \n"
"                 *##                               ##*                \n"
"                 ###                               ###                \n"
"                 ###                               ###                \n"
"                 *##                               ##*                \n"
"                   ###                           ###*                 \n"
"                    ###                         ###*                  \n"
"                     ###*                     *###                    \n"
"                      *##*                   *##*                     \n"
"                       ###                   ###                      \n"
"                       ###                   ###                      \n"
"                        ##*                 *##*                      \n"
"                         #####################                        \n"
"                           *###############*                          \n"
"                          *******************                         \n"
"                          *#################*                         \n"
"                           *****************                          \n"
"                            *              *                          \n"
"                             **************                           \n";

char onStateImage[] =
"                              **#######**                             \n"
"                          *#################*                         \n"
"                       *#######################*                      \n"
"                     *###########################*                    \n"
"                   #################################                  \n"
"                  ###################################                 \n"
"                 *###################################*                \n"
"                 #####################################                \n"
"                 #####################################                \n"
"                 *###################################*                \n"
"                   #################################*                 \n"
"                    ###############################*                  \n"
"                     #############################                    \n"
"                      *#########################*                     \n"
"                       #########################                      \n"
"                       #########################                      \n"
"                        #######################*                      \n"
"                         #####################                        \n"
"                           *###############*                          \n"
"                          *******************                         \n"
"                          *#################*                         \n"
"                          *******************                         \n"
"                            *              *                          \n"
"                             **************                           \n";

char unknownStateImage[] =
"                              **#######**                             \n"
"                          *#################*                         \n"
"                       *#####*************#####*                      \n"
"                     *####*******************####*                    \n"
"                   ###***************************###                  \n"
"                  ###*****************************###                 \n"
"                 *###*****************************###*                \n"
"                 ###*******************************###                \n"
"                 ###*******************************###                \n"
"                 *###*****************************###*                \n"
"                   ###***************************###*                 \n"
"                    ####***********************####*                  \n"
"                     ####*********************####                    \n"
"                      *###*******************###*                     \n"
"                       ###*******************###                      \n"
"                       ###*******************###                      \n"
"                        ###*****************###*                      \n"
"                         #####################                        \n"
"                           *###############*                          \n"
"                          *******************                         \n"
"                          *#################*                         \n"
"                          *******************                         \n"
"                            *              *                          \n"
"                             **************                           \n";

// clang-format on

//-----------------------------------------------------------------------------
static void
_manufacturer(vs_device_manufacture_id_t manufacture_id) {
    memset(manufacture_id, 0, sizeof(vs_device_manufacture_id_t));
    strncpy((char *)manufacture_id, "yiot-dev", sizeof(vs_device_manufacture_id_t) - 1);
}

//-----------------------------------------------------------------------------
static void
_type(vs_device_type_t device_type) {
    memset(device_type, 0, sizeof(vs_device_type_t));
    device_type[0] = 1;
}

//-----------------------------------------------------------------------------
static void
_serial(vs_device_serial_t serial) {
    memset(serial, 0, sizeof(vs_device_serial_t));
}

//-----------------------------------------------------------------------------
static vs_status_e
_info_cb(const vs_netif_t *netif, char *state, const uint16_t state_buf_sz, uint16_t *state_sz) {
    CHECK_NOT_ZERO_RET(state, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(state_sz, VS_CODE_ERR_ZERO_ARGUMENT);
    CHECK_NOT_ZERO_RET(state_buf_sz, VS_CODE_ERR_ZERO_ARGUMENT);

    nlohmann::json stateJson;
    stateJson["type"] = 1; // TODO: Get this value from common file
    stateJson["command"] = "info";
    stateJson["state"] = false;

    auto jsonStr = stateJson.dump();

    if (jsonStr.size() > state_buf_sz) {
        return VS_CODE_ERR_TOO_SMALL_BUFFER;
    }
    strcpy(state, jsonStr.c_str());
    *state_sz = strlen(state) + 1;

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_command_cb(const vs_netif_t *netif, vs_mac_addr_t sender_mac, const char *json) {
    CHECK_NOT_ZERO_RET(json, VS_CODE_ERR_NULLPTR_ARGUMENT);
    uint16_t json_sz = strnlen(json, PC_JSON_SZ_MAX);
    CHECK_RET(json_sz < PC_JSON_SZ_MAX, VS_CODE_ERR_INCORRECT_ARGUMENT, "Command request too long");

    VS_LOG_DEBUG("New command: %s", json);

    bool res = false;

    try {
        auto jsonObj = nlohmann::json::parse(json);
        auto command = jsonObj["command"].get<std::string>();
        CHECK_RET(command == "setState", VS_CODE_ERR_INCORRECT_ARGUMENT, "Wrong command");

        auto state = jsonObj["state"].get<bool>();

        if (state) {
            VS_LOG_INFO("Lamp is turned on");
        } else {
            VS_LOG_INFO("Lamp is turned off");
        }

        res = true;
    } catch (...) {
        VS_LOG_WARNING("Command cannot be processed");
    }

    return res ? VS_CODE_OK : VS_CODE_ERR_USER;
}

//-----------------------------------------------------------------------------
static const char *
_description(void) {
    return "Lamp emulator";
}

//-----------------------------------------------------------------------------
static const char *
_default_name(void) {
    return "My new Lamp";
}

//-----------------------------------------------------------------------------
ks_test_device_t *
ks_lamp(void) {
    _lamp.get_manufacturer = _manufacturer;
    _lamp.get_type = _type;
    _lamp.get_serial = _serial;
    _lamp.get_description = _description;
    _lamp.get_default_name = _default_name;
    _lamp.info_cb = _info_cb;
    _lamp.command_cb = _command_cb;

    return &_lamp;
}

//-----------------------------------------------------------------------------
