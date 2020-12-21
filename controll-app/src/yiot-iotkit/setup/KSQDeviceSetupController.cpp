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

#include <yiot-iotkit/setup/KSQDeviceSetupController.h>
#include <yiot-iotkit/snap/KSQSnapSCRTClient.h>
#include <yiot-iotkit/KSQIoTKitFacade.h>

//-----------------------------------------------------------------------------
KSQDeviceSetupController::KSQDeviceSetupController() {
    m_valid = false;

    // SNAP::INFO signals
    connect(&VSQSnapInfoClient::instance(),
            &VSQSnapInfoClient::fireDeviceInfo,
            this,
            &KSQDeviceSetupController::onDeviceInfo);

    // SNAP::SCRT signals
    connect(&KSQSnapSCRTClient::instance(),
            &KSQSnapSCRTClient::fireInfo,
            this,
            &KSQDeviceSetupController::onDeviceSecurityInfo);

    // SNAP::_CFG signals
    connect(&KSQIoTKitFacade::instance().snapCfgClient(),
            &VSQSnapCfgClient::fireConfigurationDone,
            this,
            &KSQDeviceSetupController::onConfigurationDone);

    connect(&KSQIoTKitFacade::instance().snapCfgClient(),
            &VSQSnapCfgClient::fireConfigurationError,
            this,
            &KSQDeviceSetupController::onConfigurationError);
}

//-----------------------------------------------------------------------------
KSQDeviceSetupController::~KSQDeviceSetupController() {

}

//-----------------------------------------------------------------------------
bool
KSQDeviceSetupController::start(QSharedPointer<VSQNetifBase> netif, VSQMac deviceMac) {
    m_netif = netif;
    m_deviceMac = deviceMac;

    m_valid = false;
    m_netif = netif;
    m_deviceMac = deviceMac;
    m_readyDeviceInfo = false;
    m_readyDeviceSecurityInfo = false;

    auto bleNetif = m_netif->lowLevelNetif();

    if (!VSQSnapInfoClient::instance().onStartFullPolling(m_deviceMac, bleNetif)) {
        emit fireError(tr("Cannot request device info"));
        return false;
    }

    if (!KSQSnapSCRTClient::instance().getInfo(bleNetif, m_deviceMac)) {
        emit fireError(tr("Cannot request security device info"));
        return false;
    }

    emit fireStateInfo(tr("Request device state"));

    QTimer::singleShot(1000, [this]() {
        onConfigurationDone();
    });

    QTimer::singleShot(2000, [this]() {
        VSQDeviceInfo deviceInfo(broadcastMac);
        onDeviceInfo(deviceInfo);
    });

    m_valid = true;
    return m_valid;
}

//-----------------------------------------------------------------------------
bool
KSQDeviceSetupController::configure() {
    return false;
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::error(const QString & error) {
    m_valid = false;
    emit fireError(error);
    emit fireFinished(m_netif);
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::onConfigurationDone() {
    if (!m_valid) {
        return;
    }
//    emit fireFinished(m_netif);
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::onConfigurationError() {
    if (!m_valid) {
        return;
    }
    error(tr("Cannot configure WiFi"));
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::onDeviceSecurityInfo() {
    if (!m_valid) {
        return;
    }
    VS_LOG_DEBUG(">>> onDeviceSecurityInfo");
    m_readyDeviceSecurityInfo = true;
    checkInitalStep();
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::onDeviceInfo(const VSQDeviceInfo &deviceInfo) {
    if (!m_valid) {
        return;
    }
    VS_LOG_DEBUG(">>> onDeviceInfo");
    m_readyDeviceInfo = true;
    checkInitalStep();
}

//-----------------------------------------------------------------------------
bool
KSQDeviceSetupController::checkInitalStep() {
    bool ready = m_valid && m_readyDeviceInfo && m_readyDeviceSecurityInfo;
    if (!ready) {
        return false;
    }

    emit fireInitializationReady();

    return true;
}

//-----------------------------------------------------------------------------

