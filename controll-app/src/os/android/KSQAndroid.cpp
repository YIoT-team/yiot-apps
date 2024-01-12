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

#include <QtCore>
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QtAndroidExtras>

#include "os/android/KSQAndroid.h"

//-----------------------------------------------------------------------------
void
KSQAndroid::hideSplashScreen() {
    QtAndroid::hideSplashScreen();
}


//-----------------------------------------------------------------------------
void
KSQAndroid::requestPermissions() {
    const QVector<QString> permissions({
#if KS_ENABLE_BLE
        "android.permission.BLUETOOTH", "android.permission.BLUETOOTH_ADMIN",
#endif
                "android.permission.ACCESS_FINE_LOCATION", "android.permission.WRITE_EXTERNAL_STORAGE",
                "android.permission.ACCESS_WIFI_STATE", "android.permission.CHANGE_WIFI_STATE",
                "android.permission.NEARBY_WIFI_DEVICES"
    });

    for (const QString &permission : permissions) {
        auto result = QtAndroid::checkPermission(permission);
        if (result == QtAndroid::PermissionResult::Denied) {
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if (resultHash[permission] == QtAndroid::PermissionResult::Denied) {
            }
        }
    }
}

//-----------------------------------------------------------------------------
KSQWiFiNetworks
KSQAndroid::enumWifi() {
    KSQWiFiNetworks res;
    QAndroidJniObject js = QAndroidJniObject::callStaticObjectMethod("io/kutashenko/wifi/Wifi",
                                                                     "enumWifi",
                                                                     "(Landroid/content/Context;)Ljava/lang/String;",
                                                                     QtAndroid::androidContext().object());

    QJsonDocument jsonResponse = QJsonDocument::fromJson(js.toString().toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray jsonArray = jsonObject["wifi"].toArray();

    foreach (const QJsonValue &value, jsonArray) {
        QJsonObject obj = value.toObject();
        QString key = obj["ssid"].toString();
        int val = obj["rssi"].toInt();
        res[key] = val;
    }

    return res;
}

//-----------------------------------------------------------------------------
bool
KSQAndroid::openWifiSettings() {
    QAndroidJniObject::callStaticObjectMethod("io/kutashenko/wifi/Wifi",
                                              "openSettings",
                                              "(Landroid/content/Context;)Ljava/lang/Boolean;",
                                              QtAndroid::androidContext().object());

    return true;
}

//-----------------------------------------------------------------------------
QString
KSQAndroid::currentSSID() {
    QAndroidJniObject js = QAndroidJniObject::callStaticObjectMethod("io/kutashenko/wifi/Wifi",
                                                                     "currentSSID",
                                                                     "(Landroid/content/Context;)Ljava/lang/String;",
                                                                     QtAndroid::androidContext().object());

    return js.toString();
}

//-----------------------------------------------------------------------------
