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
#include <QtDBus/QtDBus>

//-----------------------------------------------------------------------------
KSQWiFiNetworks
wifi_enum() {
    KSQWiFiNetworks res;
    QDBusInterface nm("org.freedesktop.NetworkManager",
                      "/org/freedesktop/NetworkManager",
                      "org.freedesktop.NetworkManager",
                      QDBusConnection::systemBus());
    if (!nm.isValid()) {
        qFatal("D-Bus: Failed to connect to the system bus");
    }

    QDBusMessage msg = nm.call("GetDevices");
    QDBusArgument arg = msg.arguments().at(0).value<QDBusArgument>();

    if (arg.currentType() != QDBusArgument::ArrayType) {
        qFatal("D-Bus: Something went wrong with getting the device list");
    }
    QList<QDBusObjectPath> pathsLst = qdbus_cast<QList<QDBusObjectPath> >(arg);
    foreach (QDBusObjectPath p, pathsLst) {
        QDBusInterface device("org.freedesktop.NetworkManager",
                              p.path(),
                              "org.freedesktop.NetworkManager.Device",
                              QDBusConnection::systemBus());
        if (device.property("DeviceType").toInt() != 2) {
            continue;
        }
        QDBusInterface wifi_device("org.freedesktop.NetworkManager",
                                   p.path(),
                                   "org.freedesktop.NetworkManager.Device.Wireless",
                                   QDBusConnection::systemBus());
        QMap<QString, QVariant> argList;
        QDBusMessage msg = wifi_device.call("RequestScan", argList);
        QThread::sleep(2);

        msg = wifi_device.call("GetAllAccessPoints");
        QDBusArgument ap_list_arg = msg.arguments().at(0).value<QDBusArgument>();
        QList<QDBusObjectPath> ap_path_list = qdbus_cast<QList<QDBusObjectPath> >(ap_list_arg);
        foreach (QDBusObjectPath p, ap_path_list) {
            QDBusInterface ap_interface("org.freedesktop.NetworkManager",
                                        p.path(),
                                        "org.freedesktop.NetworkManager.AccessPoint",
                                        QDBusConnection::systemBus());
            res[ap_interface.property("Ssid").toString()] = ap_interface.property("Strength").toInt();
        }
    }

    return res;
}

//-----------------------------------------------------------------------------
