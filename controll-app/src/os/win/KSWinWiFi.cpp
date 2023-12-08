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

#include <KSWiFi.h>

#include <stdio.h>
#include <windows.h>
#include <wlanapi.h>

#include <virgil/iot/logger/logger.h>

static bool bWait;

#if !defined(KS_DEBUG_WIFI_SCANNER_WIN)
#define KS_DEBUG_WIFI_SCANNER_WIN 0
#endif

//-----------------------------------------------------------------------------
static void
_wlanNotification(WLAN_NOTIFICATION_DATA *wlanNotifData, VOID *p) {
    (void)p;
    if (wlanNotifData->NotificationCode == wlan_notification_acm_scan_complete) {
        bWait = false;
    } else if (wlanNotifData->NotificationCode == wlan_notification_acm_scan_fail) {
        bWait = false;
    }
}

//-----------------------------------------------------------------------------
static bool
_isVistaOrHigher() {
    OSVERSIONINFO osVersion;
    ZeroMemory(&osVersion, sizeof(OSVERSIONINFO));
    osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    if (!GetVersionEx(&osVersion)) {
        return false;
    }

    if (osVersion.dwMajorVersion >= 6) {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
KSQWiFiNetworks
wifi_enum() {
    KSQWiFiNetworks res;

    HANDLE hWlan = NULL;

    DWORD dwError = 0;
    DWORD dwSupportedVersion = 0;
    DWORD dwClientVersion = (_isVistaOrHigher() ? 2 : 1);

    GUID guidInterface;
    ZeroMemory(&guidInterface, sizeof(GUID));

    WLAN_INTERFACE_INFO_LIST *wlanInterfaceList =
            (WLAN_INTERFACE_INFO_LIST *)WlanAllocateMemory(sizeof(WLAN_INTERFACE_INFO_LIST));
    ZeroMemory(wlanInterfaceList, sizeof(WLAN_INTERFACE_INFO_LIST));

    WLAN_AVAILABLE_NETWORK_LIST *wlanNetworkList =
            (WLAN_AVAILABLE_NETWORK_LIST *)WlanAllocateMemory(sizeof(WLAN_AVAILABLE_NETWORK_LIST));
    ZeroMemory(wlanNetworkList, sizeof(WLAN_AVAILABLE_NETWORK_LIST));

    try {
        dwError = WlanOpenHandle(dwClientVersion, NULL, &dwSupportedVersion, &hWlan);
        if (dwError != ERROR_SUCCESS) {
            VS_LOG_ERROR("Unable access wireless interface");
            return res;
        }
        dwError = WlanEnumInterfaces(hWlan, NULL, &wlanInterfaceList);
        if (dwError != ERROR_SUCCESS) {
            VS_LOG_ERROR("Unable to enum wireless interfaces");
            return res;
        }

#if KS_DEBUG_WIFI_SCANNER_WIN
        VS_LOG_DEBUG("Found adapter %ws", wlanInterfaceList->InterfaceInfo[0].strInterfaceDescription);
#endif

        dwError = wlanInterfaceList->InterfaceInfo[0].isState != wlan_interface_state_not_ready;
        if (dwError) {
            if (wlanInterfaceList->dwNumberOfItems > 1) {
                // TODO: Add processing for multiple wireless cards here
                VS_LOG_DEBUG("Detected multiple wireless adapters, using default");
                guidInterface = wlanInterfaceList->InterfaceInfo[0].InterfaceGuid;
            } else {
                guidInterface = wlanInterfaceList->InterfaceInfo[0].InterfaceGuid;
            }
        } else {
            VS_LOG_ERROR("Default wireless adapter disabled");
            return res;
        }

        DWORD dwPrevNotif = 0;

        // Scan takes awhile so we need to register a callback
        dwError = WlanRegisterNotification(hWlan,
                                           WLAN_NOTIFICATION_SOURCE_ACM,
                                           TRUE,
                                           (WLAN_NOTIFICATION_CALLBACK)_wlanNotification,
                                           NULL,
                                           NULL,
                                           &dwPrevNotif);
        if (dwError != ERROR_SUCCESS) {
            VS_LOG_ERROR("Unable to register for notifications");
            return res;
        }

#if KS_DEBUG_WIFI_SCANNER_WIN
        VS_LOG_DEBUG("Scanning for nearby networks...");
#endif

        dwError = WlanScan(hWlan, &guidInterface, NULL, NULL, NULL);
        if (dwError != ERROR_SUCCESS) {
            VS_LOG_ERROR("Scan failed, check adapter is enabled");
            return res;
        }

        // Yawn...
        while (bWait) {
            Sleep(100);
        }

        // Unregister callback, don't care if it succeeds or not
        WlanRegisterNotification(hWlan, WLAN_NOTIFICATION_SOURCE_NONE, TRUE, NULL, NULL, NULL, &dwPrevNotif);

        dwError = WlanGetAvailableNetworkList(hWlan, &guidInterface, 0, NULL, &wlanNetworkList);
        if (dwError != ERROR_SUCCESS) {
            VS_LOG_ERROR("Unable to obtain network list");
            return res;
        }

        for (unsigned int i = 0; i < wlanNetworkList->dwNumberOfItems; i++) {
            const char *ssid = (const char *)wlanNetworkList->Network[i].dot11Ssid.ucSSID;
            auto ssidStr = QString::fromLocal8Bit(ssid);
            int signal = wlanNetworkList->Network[i].wlanSignalQuality;
            res[ssidStr] = KSWiFiInfo(signal);

#if KS_DEBUG_WIFI_SCANNER_WIN
            const char *security = "";

            switch (wlanNetworkList->Network[i].dot11DefaultAuthAlgorithm) {
            case DOT11_AUTH_ALGO_80211_OPEN:
            case DOT11_AUTH_ALGO_80211_SHARED_KEY:
                security = "WEP";
                break;

            case DOT11_AUTH_ALGO_WPA:
            case DOT11_AUTH_ALGO_WPA_PSK:
            case DOT11_AUTH_ALGO_WPA_NONE:
                security = "WPA";
                break;

            case DOT11_AUTH_ALGO_RSNA:
            case DOT11_AUTH_ALGO_RSNA_PSK:
                security = "WPA2";
                break;

            default:
                security = "UNKNOWN";
                break;
            }

            VS_LOG_DEBUG("\nSSID:\t\t\t%s\nSIGNAL:\t\t\t%d\nSECURITY:\t\t%s\n", ssid, signal, security);
#endif
        }
    } catch (char *szError) {
        VS_LOG_DEBUG("%s \nQuitting...", szError);
    }

    if (wlanNetworkList) {
        WlanFreeMemory(wlanNetworkList);
    }

    if (wlanInterfaceList) {
        WlanFreeMemory(wlanInterfaceList);
    }

    if (hWlan) {
        WlanCloseHandle(hWlan, NULL);
    }

    return res;
}

//-----------------------------------------------------------------------------
