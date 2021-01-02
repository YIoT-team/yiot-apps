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
#include <yiot-iotkit/snap/KSQSnapPRVSClient.h>
#include <yiot-iotkit/root-of-trust/KSQRoTController.h>
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

    // SNAP::PRVS signals
    connect(&KSQSnapPRVSClient::instance(),
            &KSQSnapPRVSClient::fireProvisionStateChanged,
            this,
            &KSQDeviceSetupController::fireStateInfo);

    connect(&KSQSnapPRVSClient::instance(),
            &KSQSnapPRVSClient::fireProvisionError,
            this,
            &KSQDeviceSetupController::onProvisionError);

    connect(&KSQSnapPRVSClient::instance(),
            &KSQSnapPRVSClient::fireProvisionDone,
            this,
            &KSQDeviceSetupController::onProvisionDone);
}

//-----------------------------------------------------------------------------
KSQDeviceSetupController::~KSQDeviceSetupController() {
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::onProvisionError(QString errorStr) {
    error(errorStr);
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::onProvisionDone() {
    vs_cert_t my_cert;
    // TODO: Fill own certificate
    if (!KSQSnapSCRTClient::instance().addUser(
                m_netif->lowLevelNetif(), m_deviceMac, VS_USER_OWNER, "Owner 1", my_cert)) {
        error(tr("Cannot set self as an owner"));
    }
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::onAddUserError(QString errorStr) {
    error(errorStr);
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::onAddUserDone() {
    emit fireStateInfo(tr("Set WiFi credentials"));
    KSQIoTKitFacade::instance().snapCfgClient().onConfigureDevice();
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

    auto lowLevelNetif = m_netif->lowLevelNetif();

    // Request Device information
    if (!VSQSnapInfoClient::instance().onStartFullPolling(m_deviceMac, lowLevelNetif)) {
        emit fireError(tr("Cannot request device info"));
        return false;
    }

    // Request Device Security information
    if (!KSQSnapSCRTClient::instance().getInfo(lowLevelNetif, m_deviceMac)) {
        emit fireError(tr("Cannot request security device info"));
        return false;
    }

    emit fireStateInfo(tr("Request device state"));

    m_valid = true;
    return m_valid;
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::stop() {
    m_valid = false;
    emit fireUploadStopped();
}

//-----------------------------------------------------------------------------
bool
KSQDeviceSetupController::configure(QString ssid, QString password) {
    emit fireUploadStarted();

    KSQIoTKitFacade::instance().snapCfgClient().onSetConfigData(ssid, password);

    if (!KSQSnapPRVSClient::instance().provisionDevice(
                m_netif, m_deviceMac, KSQRoTController::instance().localRootOfTrust())) {
        VS_LOG_ERROR("Cannot do device provision.");
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::error(const QString &error) {
    m_valid = false;
    emit fireError(error);
    emit fireFinished(m_netif);
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::done() {
    m_valid = false;
    emit fireUploadDone();
    emit fireFinished(m_netif);
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::onConfigurationDone() {
    if (!m_valid) {
        return;
    }

    done();
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
KSQDeviceSetupController::onDeviceSecurityInfo(bool hasProvision,
                                               bool hasOwner,
                                               bool ownerIsYou,
                                               const KSQPublicKey &publicKey) {
    if (!m_valid) {
        return;
    }

    m_deviceData.setHasProvision(hasProvision);
    m_deviceData.setHasOwner(hasOwner);
    m_deviceData.setOwnerIsYou(ownerIsYou);
    m_deviceData.setPublicKey(publicKey);

    m_readyDeviceSecurityInfo = true;
    checkInitalStep();
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::onDeviceInfo(const VSQDeviceInfo &deviceInfo) {
    if (!m_valid || m_readyDeviceInfo) {
        return;
    }

    m_deviceData.setDeviceInfo(deviceInfo);
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
QObject *
KSQDeviceSetupController::deviceData() {
    return &m_deviceData;
}

//-----------------------------------------------------------------------------
