//
//                                  _____   _______
//                                 |_   _| |__   __|
//                                   | |  ___ | |
//                                   | | / _ \| |
//                                  _| || (_) | |
//                                 |_____\___/|_|
//
//    _  ________ ______ _____    _____ _______    _____ _____ __  __ _____  _      ______
//   | |/ /  ____|  ____|  __ \  |_   _|__   __|  / ____|_   _|  \/  |  __ \| |    |  ____|
//   | ' /| |__  | |__  | |__) |   | |    | |    | (___   | | | \  / | |__) | |    | |__
//   |  < |  __| |  __| |  ___/    | |    | |     \___ \  | | | |\/| |  ___/| |    |  __|
//   | . \| |____| |____| |       _| |_   | |     ____) |_| |_| |  | | |    | |____| |____
//   |_|\_\______|______|_|      |_____|  |_|    |_____/|_____|_|  |_|_|    |______|______|
//
//
//
//   21 July 2020
//   Lead Maintainer: Roman Kutashenko <kutashenko@gmail.com>

#include <unistd.h>
#include <virgil/iot/logger/logger.h>
#include <virgil/iot/macros/macros.h>

/******************************************************************************/
int
main(int argc, char *argv[]) {
    int res = -1;

    // Implementation variables

    // Initialize Logger module
    vs_logger_init(VS_LOGLEV_DEBUG);

    //
    // ---------- Create implementations ----------
    //

    // ---------- Initialize IoTKit internals ----------
    //

    // Initialize IoTKit

    //
    // ---------- Application work ----------
    //

    // Sleep until CTRL_C

    //
    // ---------- Terminate application ----------
    //

    res = 0;

    // terminate:

    VS_LOG_INFO("\n\n\n");
    VS_LOG_INFO("Terminating application ...");

    return res;
}

/******************************************************************************/
