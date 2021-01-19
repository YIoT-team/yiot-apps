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

#include <virgil/iot/qt/VSQIoTKit.h>
#include <cstring>

#include <yiot-iotkit/snap/KSQSnapPCClient.h>

using namespace VirgilIoTKit;

const QString KSQSnapPCClient::kKeyInet = "inet";
const QString KSQSnapPCClient::kKeyWiFi = "wifi";
const QString KSQSnapPCClient::kKeyIPv4WiFi = "wifi_ipv4";
const QString KSQSnapPCClient::kKeyIPv4Eth0 = "eth_ipv4";

const QString KSQSnapPCClient::kWiFiAP = "ap";
const QString KSQSnapPCClient::kWiFiSTA = "sta";

//-----------------------------------------------------------------------------
KSQSnapPCClient::KSQSnapPCClient() {
    vs_snap_pc_client_service_t impl;
    memset(&impl, 0, sizeof(impl));
    impl.device_state_update = &KSQSnapPCClient::onUpdateState;
    m_snapService = vs_snap_pc_client(impl);
}


//-----------------------------------------------------------------------------
bool
KSQSnapPCClient::getBool(const QJsonObject &obj, const QString &field) {
    if (!obj.contains(field)) {
        throw std::runtime_error("JSON doesn't contain: " + field.toStdString());
    }
    return obj[field].toBool();
}

//-----------------------------------------------------------------------------
vs_snap_pc_wifi_mode_e
KSQSnapPCClient::getWiFiType(const QJsonObject &obj, const QString &field) {
    if (!obj.contains(field)) {
        throw std::runtime_error("JSON doesn't contain: " + field.toStdString());
    }

    auto strWiFi = obj[field].toString();

    if (kWiFiAP == strWiFi) {
        return KS_PC_WIFI_AP;
    } else if (kWiFiSTA == strWiFi) {
        return KS_PC_WIFI_STA;
    }

    throw std::runtime_error("JSON contain an incorrect WiFi state: " + field.toStdString() + ":" +
                             strWiFi.toStdString());
}

//-----------------------------------------------------------------------------
uint32_t
KSQSnapPCClient::getIPv4(const QJsonObject &obj, const QString &field) {
    if (!obj.contains(field)) {
        throw std::runtime_error("JSON doesn't contain: " + field.toStdString());
    }

    auto strIPv4 = obj[field].toString();
    auto addr = QHostAddress(strIPv4);

    if (addr.isNull()) {
        throw std::runtime_error("JSON contain an incorrect ip: " + field.toStdString() + ":" + strIPv4.toStdString());
    }

    return addr.toIPv4Address();
}

//-----------------------------------------------------------------------------
vs_status_e
KSQSnapPCClient::onUpdateState(vs_status_e res, const vs_mac_addr_t *mac, const char *json) {
    bool isError = true;
    if (VS_CODE_OK == res && json) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(json);
        if (jsonResponse.isObject()) {
            auto obj = jsonResponse.object();
            vs_snap_pc_state_t data;
            try {
                data.internet_present = getBool(obj, kKeyInet) ? 1 : 0;
                data.wifi_mode = getWiFiType(obj, kKeyWiFi);
                data.wifi_ipv4 = getIPv4(obj, kKeyIPv4WiFi);
                data.ethernet_ipv4 = getIPv4(obj, kKeyIPv4Eth0);
                emit KSQSnapPCClient::instance().fireStateUpdate(*mac, data);
                isError = false;
            } catch (const std::runtime_error &e) {
                qDebug() << "Parsing error: " + QString::fromUtf8(e.what());
            }
        }
    }

    if (isError) {
        emit KSQSnapPCClient::instance().fireStateError(*mac);
    }

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
void
KSQSnapPCClient::requestState(const vs_mac_addr_t &mac) {
    vs_snap_pc_get_state(vs_snap_netif_routing(), &mac);
}

//-----------------------------------------------------------------------------
bool
KSQSnapPCClient::sendCommand(QString mac, QString json) {
    auto macVal = VSQMac(mac);

    if (macVal == invalidMac) {
        VS_LOG_WARNING("Cannot send command to an invalid MAC address");
        return false;
    }

    vs_mac_addr_t macLowLevel = macVal;
    return VS_CODE_OK == vs_snap_pc_command(vs_snap_netif_routing(), &macLowLevel, json.toStdString().c_str());
}

//-----------------------------------------------------------------------------