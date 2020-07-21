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

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/******************************************************************************/
void
vs_impl_msleep(size_t msec) {
    usleep(msec * 1000);
}

/******************************************************************************/
bool
vs_logger_output_hal(const char *buffer) {
    if (!buffer) {
        return false;
    }

    int res = printf("%s", buffer) != 0;
    fflush(stdout);
    return res != 0;
}

/******************************************************************************/
bool
vs_logger_current_time_hal(void) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char buf[50];
    size_t len;
    snprintf(buf, sizeof(buf), "%s", asctime(timeinfo));
    len = strlen(buf);

    if (!len) {
        return false;
    }

    buf[len - 1] = 0;
    int res = printf("[%s]", buf);
    return res != 0;
}

/******************************************************************************/
