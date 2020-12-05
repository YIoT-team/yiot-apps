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

#include "keychain.h"

#ifdef Q_OS_ANDROID
#include "android/KSQAndroid.h"
#endif // Q_OS_ANDROID

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

//-----------------------------------------------------------------------------
int
KSQApplication::run() {
    QQmlApplicationEngine engine;
    VSQUiHelper uiHelper;
    m_netifUdp = QSharedPointer<KSQUdp>::create();

    // Prepare IoTKit data
    auto features = KSQFeatures() << KSQFeatures::SNAP_PRVS_CLIENT  // Required to provision devices
                                  << KSQFeatures::SNAP_SCRT_CLIENT  // Secure communication (DH, AES, etc)
                                  << KSQFeatures::SNAP_INFO_CLIENT  // Get/set device information (Type, Name, Versions)
                                  << KSQFeatures::SNAP_CFG_CLIENT   // Configure device (Different credentials, WiFi for example)
                                  << KSQFeatures::SNAP_LAMP_CLIENT  // Possibility to control Lamp
                                  << KSQFeatures::SNAP_PC_CLIENT;   // Possibility to control PSs, Raspberry Pi for example

    // TODO: Dynamic adding of supported network interfaces
    auto impl = VSQImplementations() << m_netifUdp                  // Enables UDP communication
                                     << m_bleController.netif();    // Enables Bluetooth Low Energy communication

    // This is a control device
    auto roles = VSQDeviceRoles() << VirgilIoTKit::VS_SNAP_DEV_CONTROL;

    // Set different information about current device
    auto appConfig = VSQAppConfig() << VSQManufactureId() << VSQDeviceType() << VSQDeviceSerial()
                                    << VirgilIoTKit::VS_LOGLEV_DEBUG << roles;

    // Initialize IoTKit
    if (!KSQIoTKitFacade::instance().init(features, impl, appConfig)) {
        VS_LOG_CRITICAL("Unable to initialize IoTKIT");
        return -1;
    }

    // Initialize devices controllers
    //          TODO: Dynamic adding of supported devices
    m_deviceControllers << new KSQLampController()      // Possibility to control lamps
                        << new KSQPCController();       // Possibility to control PSs, like Raspberry Pi

    // Initialize QML
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("UiHelper", &uiHelper);
    context->setContextProperty("app", this);                           // Get app name, version, etc.
    context->setContextProperty("bleController", &m_bleController);     // Connect/disconnect to BLE devices to communicate with
    context->setContextProperty("bleEnum", m_bleController.model());    // BLE device enumeration // TODO: Use from `bleController`
    context->setContextProperty("wifiEnum", &m_wifiEnumerator);         // WiFi networks enumeration
    context->setContextProperty("deviceControllers", &m_deviceControllers); // Containers with controllers for all supported devices
    context->setContextProperty("rotModel", &m_rot);                    // Container for all Roots of trust

    // Load UI theme
    qmlRegisterSingletonType(QUrl("qrc:/qml/theme/Theme.qml"), "Theme", 1, 0, "Theme");

    // QML entrypoint
    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));

    // Start QML
    engine.load(url);

    // Delayed actions
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
    // Multiple broadcast requests due to UDP specific
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
KSQApplication::applicationVersion() const {
    return tr("version: ")
           + QString(TOSTRING(KS_TARGET_VERSION)) + "."
           + QString(TOSTRING(KS_BUILD_NUMBER));
}

//-----------------------------------------------------------------------------
QString
KSQApplication::applicationDisplayName() const {
    return tr("YIoT");
}

//-----------------------------------------------------------------------------
