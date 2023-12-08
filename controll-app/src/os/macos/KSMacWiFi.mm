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
//  ────────────────────────────────────────────────────────────>

#include <KSWiFi.h>
#import <CoreWLAN/CoreWLAN.h>

#if !defined(KS_DEBUG_WIFI_SCANNER_MAC)
#define KS_DEBUG_WIFI_SCANNER_MAC 0
#endif

//-----------------------------------------------------------------------------
KSQWiFiNetworks
wifi_enum() {
    KSQWiFiNetworks res;
    CWInterface* wifi = [[CWWiFiClient sharedWiFiClient] interface];
    NSArray *networkScan = [[wifi scanForNetworksWithName:nil error:nil] allObjects];
    for (CWNetwork *network in networkScan) {
        #if KS_DEBUG_WIFI_SCANNER_MAC
        NSLog ( @"SSID: %@ ,\n \
              BSSID: %@ , \n \
              rssiValue: %ld , \n \
              noiseMeasurement: %ld, \n\
              beaconInterval: %ld , \n \
              countryCode: %@ \n ,\
              ibss: %i ,\n\
              wlanChannel: %@ , \n\
              ", [network ssid],[network bssid],[network rssiValue],[network noiseMeasurement],(long)[network beaconInterval], [network countryCode] , [network ibss], [[network wlanChannel]description]);
        #endif
        const auto key = QString::fromNSString([network ssid]);
        res[key] = KSWiFiInfo([network rssiValue]);
    }

    return res;
}

//-----------------------------------------------------------------------------
