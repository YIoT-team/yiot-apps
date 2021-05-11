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
    emit fireRequestDeviceSetup(device.get());
}

//-----------------------------------------------------------------------------
bool
KSQUXSimplifyController::startDeviceProvision(QString name) {
    qDebug() << "startDeviceProvision : " << name;

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
