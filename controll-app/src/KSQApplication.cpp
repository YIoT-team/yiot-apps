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
#include <virgil/iot/logger/logger.h>

#include <yiot-iotkit/KSQIoTKitFacade.h>
#include <yiot-iotkit/setup/KSQDeviceSetupController.h>
#include <yiot-iotkit/snap/KSQSnapUSERClient.h>
#include <yiot-iotkit/provision/KSQProvision.h>

#include <virgil/iot/qt/protocols/snap/VSQSnapCFGClient.h>

#include "keychain.h"

#ifdef Q_OS_ANDROID
#include "os/android/KSQAndroid.h"
#endif // Q_OS_ANDROID

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

const QString KSQApplication::kWebSocketIntegrationID = "io.yiot-dev.websocketrouter";

//-----------------------------------------------------------------------------
int
KSQApplication::run() {
    QQmlApplicationEngine engine;

    // Early initialization of logger
    vs_logger_init(VirgilIoTKit::VS_LOGLEV_DEBUG);

#if KS_ENABLE_BLE
    m_bleController = QSharedPointer<KSQBLEController>::create();
#endif
    m_netifUdp = new KSQUdp(QHostAddress::Broadcast);
    m_netifWebsock = new KSQNetifWebsocket();
    m_localBlankDevicesController = QSharedPointer<KSQBlankDevicesController>::create();
    m_uxController = QSharedPointer<KSQUXSimplifyController>::create();
    m_deviceControllers = QSharedPointer<KSQAllDevicesController>::create();
    m_integrations = QSharedPointer<KSQIntegrationsController>::create();
    m_plugins = QSharedPointer<KSQPluginsController>::create();
    m_extensionDevices = QSharedPointer<KSQExtensions>::create("devices", m_deviceControllers);
    m_extensionPlugins = QSharedPointer<KSQExtensions>::create("plugins", m_plugins);
    m_extensionIntegrations = QSharedPointer<KSQExtensions>::create("integrations", m_integrations);

    // Prepare IoTKit data
    auto features =
            KSQFeatures() << KSQFeatures::SNAP_PRVS_CLIENT // Required to provision devices
                          << KSQFeatures::SNAP_SCRT_CLIENT // Secure communication (DH, AES, etc)
                          << KSQFeatures::SNAP_INFO_CLIENT // Get/set device information (Type, Name, Versions)
                          << KSQFeatures::SNAP_CFG_CLIENT  // Configure device (Different credentials, WiFi for example)
                          << KSQFeatures::SNAP_PC_CLIENT;  // Possibility to control PSs, Raspberry Pi for example

    // TODO: Dynamic adding of supported network interfaces
    auto impl = VSQImplementations() << m_netifUdp     // Enables UDP communication
                                     << m_netifWebsock // Enable WebSocket communication
#if KS_ENABLE_BLE
                                     << m_bleController->netif() // Enables Bluetooth Low Energy communication
#endif
            ;

    // This is a control device
    auto roles = VSQDeviceRoles() << VirgilIoTKit::VS_SNAP_DEV_CONTROL;

    // Set different information about current device
    auto appConfig = VSQAppConfig() << VSQManufactureId() << VSQDeviceType() << VSQDeviceSerial() << roles
                                    << VS_LOGLEV_DEBUG;

    // Connect User Experience simplifier
    //      Get information about close BLE devices
#if KS_ENABLE_BLE
    connect(m_bleController->model(),
            &VSQNetifBLEEnumerator::fireDeviceIsClose,
            m_uxController.get(),
            &KSQUXSimplifyController::onBLEDeviceIsClose);
    connect(m_bleController.get(),
            &KSQBLEController::fireStartConnection,
            m_uxController.get(),
            &KSQUXSimplifyController::onBLEDeviceConnection);
#endif

    //      Get information about devices those require provision
    connect(m_localBlankDevicesController.get(),
            &KSQBlankDevicesController::fireDeviceRequiresProvision,
            m_uxController.get(),
            &KSQUXSimplifyController::onDeviceRequiresProvision);

    //      Get information about devices new but provisioned devices
    connect(m_deviceControllers.get(),
            &KSQAllDevicesController::fireNewProvisionedDevice,
            m_uxController.get(),
            &KSQUXSimplifyController::onNewProvisionedDevice);

    connect(m_deviceControllers.get(),
            &KSQAllDevicesController::fireNewUnknownDevice,
            this,
            &KSQApplication::updateDevices);

    // Device re-scan on provision finish
#if KS_ENABLE_BLE
    connect(m_bleController.get(), &KSQBLEController::fireProvisionDone, this, &KSQApplication::onProvisionDone);
#endif

    // Connect signals from network interfaces
    connect(m_netifWebsock, &KSQNetifWebsocket::fireDeviceReady, this, &KSQApplication::updateDevices);

    // Connect Integrations signals
    connect(m_integrations.get(),
            &KSQIntegrationsController::fireActivate,
            this,
            &KSQApplication::onIntegrationActivate);
    connect(m_integrations.get(),
            &KSQIntegrationsController::fireDeactivate,
            this,
            &KSQApplication::onIntegrationDeactivate);

    // Initialize IoTKit
    if (!KSQIoTKitFacade::instance().init(features, impl, appConfig)) {
        VS_LOG_CRITICAL("Unable to initialize IoTKIT");
        return -1;
    }

    // Root of trust changes
    connect(&KSQRoTController::instance(),
            &KSQRoTController::fireRoTUpdated,
            &KSQProvision::instance(),
            &KSQProvision::onRoTUpdated);

    // Initialize QML
    QQmlContext *context = engine.rootContext();

    context->setContextProperty("app", this); // Get app name, version, etc.
    context->setContextProperty("localBlankDevicesController", m_localBlankDevicesController.get());
#if KS_ENABLE_BLE
    context->setContextProperty("bleController",
                                m_bleController.get()); // Connect/disconnect to BLE devices to communicate with
    context->setContextProperty("bleEnum",
                                m_bleController->model()); // BLE device enumeration // TODO: Use from `bleController`
#endif
    context->setContextProperty("wifiEnum", &m_wifiEnumerator); // WiFi networks enumeration
    context->setContextProperty("extensionDevices", m_extensionDevices.get());
    context->setContextProperty("extensionPlugins", m_extensionPlugins.get());
    context->setContextProperty("extensionIntegrations", m_extensionIntegrations.get());
    context->setContextProperty("deviceControllers",
                                m_deviceControllers.get()); // Containers with controllers for all supported devices
    context->setContextProperty(
            "deviceSetup",                          // Device setup state-machine
            &KSQDeviceSetupController::instance()); // Controller to setup device provision, the first owner etc.
    context->setContextProperty("rotModel", &KSQRoTController::instance()); // Container for all Roots of trust
    context->setContextProperty("uxSimplifier", m_uxController.get());      // User experience simplifier

    // Load UI theme
    qmlRegisterSingletonType(QUrl("qrc:/qml/theme/Theme.qml"), "Theme", 1, 0, "Theme");

    // Load UI platform info
    qmlRegisterSingletonType(QUrl("qrc:/qml/base/Platform.qml"), "Platform", 1, 0, "Platform");

    // QML entrypoint
    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));

    // Start QML
    engine.load(url);

    // Initialize extensions
    m_integrations->setQmlEngine(&engine);
    m_extensionIntegrations->load();

    // Initialize devices controllers
    m_deviceControllers->setQmlEngine(&engine);
    m_extensionDevices->load();

    // Initialize device plugins
    m_plugins->setQmlEngine(&engine);
    m_extensionPlugins->load();

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
KSQApplication::onProvisionDone(QString mac) {
    updateDevices();
}

//-----------------------------------------------------------------------------
void
KSQApplication::updateDevices() {
    if (m_updateTimer.isActive()) {
        return;
    }

    m_updateTimer.setInterval(4000);
    m_updateTimer.setSingleShot(false);

    connect(&m_updateTimer, &QTimer::timeout, []() { KSQIoTKitFacade::instance().updateAll(); });

    KSQIoTKitFacade::instance().updateAll();
    m_updateTimer.start();
}

//-----------------------------------------------------------------------------
void
KSQApplication::setSubnet(QString subnet) {
    QHostAddress addr;
    auto tmp = QHostAddress(subnet);

    if (tmp.isBroadcast() || tmp.isGlobal() || tmp.isLinkLocal()) {
        addr = tmp;
    } else {
        addr = QHostAddress(m_wifiEnumerator.routeIp());
    }

    m_netifUdp->setSubnet(addr);
}

//-----------------------------------------------------------------------------
void
KSQApplication::openWiFiSettings() {
#ifdef Q_OS_ANDROID
    KSQAndroid::openWifiSettings();
#elif defined(Q_OS_IOS)
    m_wifiEnumerator.settings();
#endif
}

//-----------------------------------------------------------------------------
QString
KSQApplication::wifiSSID() {
#ifdef Q_OS_ANDROID
    return KSQAndroid::currentSSID().replace("\"", "");
#endif
    return m_wifiEnumerator.ssid();
}

//-----------------------------------------------------------------------------
QString
KSQApplication::organizationDisplayName() const {
    return tr("Your IoT");
}

//-----------------------------------------------------------------------------
QString
KSQApplication::applicationVersion() const {
    return tr("version: ") + QString(TOSTRING(KS_TARGET_VERSION)) + "." + QString(TOSTRING(KS_BUILD_NUMBER));
}

//-----------------------------------------------------------------------------
QString
KSQApplication::applicationDisplayName() const {
    return tr("EnGenius");
}

//-----------------------------------------------------------------------------
void
KSQApplication::onIntegrationActivate(QString integrationId, QString message) {
    qDebug() << "Integration Activate : " << integrationId << " message : " << message;

    if (kWebSocketIntegrationID == integrationId) {
        m_netifWebsock->connectWS(message);
    }
}

//-----------------------------------------------------------------------------
void
KSQApplication::onIntegrationDeactivate(QString integrationId) {
    qDebug() << "Integration Deactivate : " << integrationId;

    if (kWebSocketIntegrationID == integrationId) {
        m_netifWebsock->disconnectWS();
    }
}

//-----------------------------------------------------------------------------
