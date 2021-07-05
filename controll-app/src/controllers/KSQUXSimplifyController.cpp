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

#include <controllers/KSQUXSimplifyController.h>

#include <yiot-iotkit/setup/KSQDeviceSetupController.h>

#include <virgil/iot/qt/VSQIoTKit.h>

//-----------------------------------------------------------------------------
KSQUXSimplifyController::KSQUXSimplifyController() {
    connect(&KSQDeviceSetupController::instance(),
            &KSQDeviceSetupController::fireDeviceSetupStarted,
            this,
            &KSQUXSimplifyController::onNewSetup);
}

//-----------------------------------------------------------------------------
void
KSQUXSimplifyController::onBLEDeviceIsClose(QString deviceMac, QString deviceName, bool requiresProvision) {
    if (requiresProvision) {
        requestProvisionUI(deviceMac, deviceName, true);
    }
}

//-----------------------------------------------------------------------------
void
KSQUXSimplifyController::onDeviceRequiresProvision(QString deviceName,
                                                   QSharedPointer<VSQNetifBase> netif,
                                                   VSQMac deviceMac) {
    m_netif = netif;
    m_deviceMac = deviceMac;
    requestProvisionUI(deviceMac.description(), deviceName, netif->requiresAdditionalActivation());
}

//-----------------------------------------------------------------------------
void
KSQUXSimplifyController::onNewProvisionedDevice(QSharedPointer<KSQDevice> device) {
    if (m_provisionedDevices.contains(device->macAddr())) {
        m_provisionedDevices.remove(device->macAddr());
        emit fireRequestDeviceSetup(device.get());
    }
}

//-----------------------------------------------------------------------------
void
KSQUXSimplifyController::onBLEDeviceConnection(QString deviceMac) {
    m_ignoredDevices << deviceMac;
}

//-----------------------------------------------------------------------------
void
KSQUXSimplifyController::onNewSetup(const VSQMac &mac) {
    qDebug() << "startDeviceProvision : " << mac.description();
    m_provisionedDevices << mac;
    m_ignoredDevices << mac;
}

//-----------------------------------------------------------------------------
bool
KSQUXSimplifyController::startDeviceProvision(QString name) {
    KSQDeviceSetupController::instance().start(m_netif, m_deviceMac);
    return true;
}

//-----------------------------------------------------------------------------
bool
KSQUXSimplifyController::rejectDeviceProvision(QString name) {
#if 0
    qDebug() << "rejectDeviceProvision : " << name;
#endif
    return true;
}

//-----------------------------------------------------------------------------
void
KSQUXSimplifyController::requestProvisionUI(const QString &deviceMac,
                                            const QString &deviceName,
                                            bool requiresAdditionalActivation) {
    if (!m_ignoredDevices.contains(deviceMac)) {
        m_ignoredDevices.insert(deviceMac);
        emit fireRequestDeviceProvision(deviceMac, deviceName, requiresAdditionalActivation);
    }
}

//-----------------------------------------------------------------------------
