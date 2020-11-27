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
#include <QtQml>

#include <KSQApplication.h>
#include <ui/VSQUiHelper.h>
#include <virgil/iot/logger/logger.h>

#include <yiot-iotkit/KSQIoTKitFacade.h>

#include <devices/lamp/KSQLampController.h>
#include <devices/pc/KSQPCController.h>

#ifdef Q_OS_ANDROID
#include "android/KSQAndroid.h"
#endif // Q_OS_ANDROID

//-----------------------------------------------------------------------------
int
KSQApplication::run() {
    QQmlApplicationEngine engine;
    VSQUiHelper uiHelper;
    m_netifUdp = QSharedPointer<KSQUdp>::create();

    // Prepare IoTKit data
    auto features = KSQFeatures() << KSQFeatures::SNAP_INFO_CLIENT << KSQFeatures::SNAP_CFG_CLIENT
                                  << KSQFeatures::SNAP_LAMP_CLIENT << KSQFeatures::SNAP_PC_CLIENT;
    auto impl = VSQImplementations() << m_netifUdp << m_bleController.netif();
    auto roles = VSQDeviceRoles() << VirgilIoTKit::VS_SNAP_DEV_CONTROL;
    auto appConfig = VSQAppConfig() << VSQManufactureId() << VSQDeviceType() << VSQDeviceSerial()
                                    << VirgilIoTKit::VS_LOGLEV_DEBUG << roles;

    // Initialize IoTKit
    if (!KSQIoTKitFacade::instance().init(features, impl, appConfig)) {
        VS_LOG_CRITICAL("Unable to initialize IoTKIT");
        return -1;
    }

    // Initialize devices controllers
    m_deviceControllers << new KSQLampController() << new KSQPCController();

    // Initialize QML
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("UiHelper", &uiHelper);
    context->setContextProperty("app", this);
    context->setContextProperty("bleController", &m_bleController);
    context->setContextProperty("bleEnum", m_bleController.model());
    context->setContextProperty("wifiEnum", &m_wifiEnumerator);
    context->setContextProperty("deviceControllers", &m_deviceControllers);
    qmlRegisterSingletonType(QUrl("qrc:/qml/theme/Theme.qml"), "Theme", 1, 0, "Theme");
    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
    engine.load(url);

    QTimer::singleShot(200, []() {
#ifdef Q_OS_ANDROID
        KSQAndroid::hideSplashScreen();
        KSQAndroid::requestPermissions();
#endif
    });

    return QGuiApplication::instance()->exec();
}

//-----------------------------------------------------------------------------
void
KSQApplication::updateDevices() {
    KSQIoTKitFacade::instance().updateAll();
    QTimer::singleShot(3000, []() { KSQIoTKitFacade::instance().updateAll(); });
    QTimer::singleShot(5000, []() { KSQIoTKitFacade::instance().updateAll(); });
}

//-----------------------------------------------------------------------------
QString
KSQApplication::organizationDisplayName() const {
    return tr("Your IoT");
}

//-----------------------------------------------------------------------------
QString
KSQApplication::applicationDisplayName() const {
    return tr("YIoT");
}

//-----------------------------------------------------------------------------
