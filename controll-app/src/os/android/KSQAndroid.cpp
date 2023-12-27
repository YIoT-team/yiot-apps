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
#include <QJniObject>
#include <QtCore/private/qandroidextras_p.h>

#include "os/android/KSQAndroid.h"

//-----------------------------------------------------------------------------
void
KSQAndroid::hideSplashScreen() {
    QNativeInterface::QAndroidApplication::hideSplashScreen();
}


//-----------------------------------------------------------------------------
void
KSQAndroid::requestPermissions() {
    const QVector<QString> permissions({"android.permission.BLUETOOTH",
                                        "android.permission.BLUETOOTH_ADMIN",
                                        "android.permission.WRITE_EXTERNAL_STORAGE",
                                        "android.permission.ACCESS_FINE_LOCATION",
                                        "android.permission.ACCESS_WIFI_STATE",
                                        "android.permission.CHANGE_WIFI_STATE"});

    for (const QString &permission : permissions) {
        auto r = QtAndroidPrivate::checkPermission(permission).result();
        if (r == QtAndroidPrivate::Denied) {
            QtAndroidPrivate::requestPermission(permission).result();
        }
    }
}

//-----------------------------------------------------------------------------
KSQWiFiNetworks
KSQAndroid::enumWifi() {
    KSQWiFiNetworks res;
    QJniObject js = QJniObject::callStaticObjectMethod("io/kutashenko/wifi/Wifi",
                                                       "enumWifi",
                                                       "(Landroid/content/Context;)Ljava/lang/String;",
                                                       QNativeInterface::QAndroidApplication::context());

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
