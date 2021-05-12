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
    connect(&KSQDeviceSetupController::instance(), &KSQDeviceSetupController::fireDeviceSetupStarted,
            this, &KSQUXSimplifyController::onNewSetup);
}

//-----------------------------------------------------------------------------
void
KSQUXSimplifyController::onBLEDeviceIsClose(QString deviceName, bool requiresProvision) {
    if (requiresProvision) {
        requestProvisionUI(deviceName);
    }
}

//-----------------------------------------------------------------------------
void
KSQUXSimplifyController::onDeviceRequiresProvision(QString deviceName,
                                                   QSharedPointer<VSQNetifBase> netif,
                                                   VSQMac deviceMac) {
    m_netif = netif;
    m_deviceMac = deviceMac;
    requestProvisionUI(deviceName);
}

//-----------------------------------------------------------------------------
void
KSQUXSimplifyController::onNewProvisionedDevice(QSharedPointer<KSQDeviceBase> device) {
    if (m_provisionedDevices.contains(device->macAddr())) {
        m_provisionedDevices.remove(device->macAddr());
        emit fireRequestDeviceSetup(device.get());
    }
}

//-----------------------------------------------------------------------------
void
KSQUXSimplifyController::onNewSetup(const VSQMac &mac) {
    qDebug() << "startDeviceProvision : " << mac.description();
    m_provisionedDevices << mac;
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
KSQUXSimplifyController::requestProvisionUI(const QString &deviceName) {
    if (!m_ignoredDevices.contains(deviceName)) {
        emit fireRequestDeviceProvision(deviceName);
        m_ignoredDevices.insert(deviceName);
    }
}

//-----------------------------------------------------------------------------
