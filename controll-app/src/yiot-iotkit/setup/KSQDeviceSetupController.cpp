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

    connect(&KSQSnapSCRTClient::instance(),
            &KSQSnapSCRTClient::fireUserAddDone,
            this,
            &KSQDeviceSetupController::onAddUserDone);

    connect(&KSQSnapSCRTClient::instance(),
            &KSQSnapSCRTClient::fireUserAddError,
            this,
            &KSQDeviceSetupController::onAddUserError);

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
    static const size_t kCertMaxSz = sizeof(vs_cert_t) + 1024;
    uint8_t certBuf[kCertMaxSz];
    vs_cert_t *myCert = reinterpret_cast<vs_cert_t *>(certBuf);

    if (VS_CODE_OK != vs_provision_own_cert(myCert, kCertMaxSz)) {
        VS_LOG_ERROR("Cannot get own certificate");
        emit fireError(tr("Cannot get own certificate"));
        return;
    }

    if (m_needUser) {
        emit fireStateInfo(tr("Set device owner"));
        auto lowLevelNetif = m_netif.isNull() ? vs_snap_netif_routing() : m_netif->lowLevelNetif();
        if (!KSQSnapSCRTClient::instance().addUser(lowLevelNetif, m_deviceMac, VS_USER_OWNER, m_userName, *myCert)) {
            error(tr("Cannot set self as an owner"));
        }
    } else {
        done();
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
    if (!m_valid) {
        return;
    }

    if (m_needWiFi) {
        emit fireStateInfo(tr("Set WiFi credentials"));
        KSQIoTKitFacade::instance().snapCfgClient().onConfigureDevice(m_netif, m_deviceMac);
    } else {
        done();
    }
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


    auto lowLevelNetif = vs_snap_default_netif();
    if (!m_netif.isNull()) {
        lowLevelNetif = m_netif->lowLevelNetif();
    }

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

    emit fireDeviceSetupStarted(deviceMac);

    m_valid = true;
    return m_valid;
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupController::stop() {
    m_valid = false;
    emit fireUploadStopped();
    emit fireFinished(m_netif);
}

//-----------------------------------------------------------------------------
bool
KSQDeviceSetupController::configure(bool needProvision,
                                    bool needUser,
                                    QString userName,
                                    bool needWiFi,
                                    QString ssid,
                                    QString password) {
    emit fireUploadStarted();

    m_needProvision = needProvision;
    m_needUser = needUser;
    m_needWiFi = needWiFi;
    m_userName = userName;

    if (needWiFi) {
        if (ssid.isEmpty() || password.isEmpty()) {
            error("Empty WiFi parameters");
            return false;
        }
        KSQIoTKitFacade::instance().snapCfgClient().onSetConfigData(ssid, password);
    }

    if (needProvision) {
        if (!KSQSnapPRVSClient::instance().provisionDevice(
                    m_netif, m_deviceMac, KSQRoTController::instance().localRootOfTrust())) {
            VS_LOG_ERROR("Cannot do device provision.");
            return false;
        }
    } else if (needUser) {
        onProvisionDone();

    } else if (needWiFi) {
        onAddUserDone();

    } else {
        done();
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
