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
#include <cstring>

#include "common/helpers/settings.h"

#define BASE_DIR "/yiot/pc"
#define SCRIPTS_DIR "/scripts"

static char *_base_dir = NULL;
static char *_scripts_dir = NULL;

//-----------------------------------------------------------------------------
static void
_prepare() {
    if (!_base_dir || !_scripts_dir) {
        const char *home = getenv("HOME");
        if (!home) {
            return;
        }
        if (_base_dir) {
            free(_base_dir);
        }

        _base_dir = static_cast<char *>(malloc(strlen(home) + strlen(BASE_DIR) + 1));
        sprintf(_base_dir, "%s%s", home, BASE_DIR);

        if (_scripts_dir) {
            free(_scripts_dir);
        }

        _scripts_dir = static_cast<char *>(malloc(strlen(home) + strlen(BASE_DIR) + strlen(SCRIPTS_DIR) + 1));
        sprintf(_scripts_dir, "%s%s%s", home, BASE_DIR, SCRIPTS_DIR);
    }
}

//-----------------------------------------------------------------------------
const char *
ks_settings_installation_dir(void) {
    _prepare();
    return _base_dir;
}

//-----------------------------------------------------------------------------
extern "C" const char *
ks_settings_scripts_dir(void) {
    _prepare();
    return _scripts_dir;
}

//-----------------------------------------------------------------------------
