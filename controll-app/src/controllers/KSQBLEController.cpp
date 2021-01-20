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

#include <controllers/KSQBLEController.h>

#include <virgil/iot/qt/VSQIoTKit.h>

#include <yiot-iotkit/KSQIoTKitFacade.h>
#include <yiot-iotkit/setup/KSQDeviceSetupController.h>

//-----------------------------------------------------------------------------
KSQBLEController::KSQBLEController() {
    m_netifBLE = QSharedPointer<VSQNetifBLE>::create();
    m_bleEnumerator.startDiscovery();

    // Connect signals and slots
    connect(m_netifBLE.data(), &VSQNetifBLE::fireDeviceReady, this, &KSQBLEController::onConnected);

    connect(m_netifBLE.data(), &VSQNetifBLE::fireDeviceDisconnected, this, &KSQBLEController::onDisconnected);

    // connect(m_netifBLE.data(), &VSQNetifBLE::fireDeviceError, this, &KSQBLEController::onDeviceError);

    connect(&KSQDeviceSetupController::instance(),
            &KSQDeviceSetupController::fireFinished,
            this,
            &KSQBLEController::onSetupFinished);
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
        KSQDeviceSetupController::instance().error(tr("Connection error"));
        return;
    }

    KSQDeviceSetupController::instance().start(netif(), broadcastMac);
}

//-----------------------------------------------------------------------------
void
KSQBLEController::cleanConnections() {
}

//-----------------------------------------------------------------------------
void
KSQBLEController::onDisconnected() {
    //    emit fireDisconnected();
    cleanConnections();
}

//-----------------------------------------------------------------------------
void
KSQBLEController::onDeviceError() {
    m_netifBLE->close();
    KSQDeviceSetupController::instance().error(tr("unknown"));
    cleanConnections();
}

//-----------------------------------------------------------------------------
void
KSQBLEController::onSetupFinished(QSharedPointer<VSQNetifBase> netif) {
    if (netif == m_netifBLE) {
        m_netifBLE->close();
    }
}

//-----------------------------------------------------------------------------
Q_INVOKABLE bool
KSQBLEController::connectDevice(const QString &deviceName) {
    cleanConnections();

    auto ble = m_bleEnumerator.devInfo(deviceName);
    if (!ble.isValid()) {
        return false;
    }


    return m_netifBLE->open(ble);
}

//-----------------------------------------------------------------------------
