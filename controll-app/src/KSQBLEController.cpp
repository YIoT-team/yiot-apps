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

#include <KSQBLEController.h>

#include <virgil/iot/qt/VSQIoTKit.h>

#include <yiot-iotkit/KSQIoTKitFacade.h>

//-----------------------------------------------------------------------------
KSQBLEController::KSQBLEController() {
    m_netifBLE = QSharedPointer<VSQNetifBLE>::create();
    m_bleEnumerator.startDiscovery();

    // Connect signals and slots
    connect(m_netifBLE.data(), &VSQNetifBLE::fireDeviceReady, this, &KSQBLEController::onConnected);

    connect(m_netifBLE.data(), &VSQNetifBLE::fireDeviceDisconnected, this, &KSQBLEController::onDisconnected);

    connect(m_netifBLE.data(), &VSQNetifBLE::fireDeviceError, this, &KSQBLEController::onDeviceError);

    connect(&KSQIoTKitFacade::instance().snapCfgClient(),
            &VSQSnapCfgClient::fireConfigurationDone,
            this,
            &KSQBLEController::onConfigurationDone);

    connect(&KSQIoTKitFacade::instance().snapCfgClient(),
            &VSQSnapCfgClient::fireConfigurationError,
            this,
            &KSQBLEController::onDeviceError);
}

//-----------------------------------------------------------------------------
KSQBLEController::~KSQBLEController() {
    m_netifBLE->close();
}

//-----------------------------------------------------------------------------
QSharedPointer<VSQNetifBLE>
KSQBLEController::netif() {
    return m_netifBLE;
}

//-----------------------------------------------------------------------------
VSQNetifBLEEnumerator *
KSQBLEController::model() {
    return &m_bleEnumerator;
}

//-----------------------------------------------------------------------------
void
KSQBLEController::onConnected(bool success) {
    if (!success) {
        emit fireError(tr("Connection error"));
        return;
    }

    emit fireConnected();

    if (m_needWiFiConfig) {
        m_needWiFiConfig = false;
        KSQIoTKitFacade::instance().snapCfgClient().onConfigureDevices();
        emit fireDataSent();
    }
}

//-----------------------------------------------------------------------------
void
KSQBLEController::onDisconnected() {
    emit fireDisconnected();
}

//-----------------------------------------------------------------------------
void
KSQBLEController::onDeviceError() {
    m_netifBLE->close();
    emit fireError(tr("unknown"));
}

//-----------------------------------------------------------------------------
void
KSQBLEController::onConfigurationDone() {
    m_netifBLE->close();
    emit fireDataReceived();
}

//-----------------------------------------------------------------------------
bool
KSQBLEController::configureWiFi(const QString &deviceName, const QString &ssid, const QString &password) {
    qDebug() << "deviceName: " << deviceName;
    qDebug() << "ssid      : " << ssid;
    qDebug() << "password  : " << password;

    auto ble = m_bleEnumerator.devInfo(deviceName);
    if (!ble.isValid()) {
        return false;
    }

    KSQIoTKitFacade::instance().snapCfgClient().onSetConfigData(ssid, password);
    m_needWiFiConfig = true;
    return m_netifBLE->open(ble);
}

//-----------------------------------------------------------------------------
