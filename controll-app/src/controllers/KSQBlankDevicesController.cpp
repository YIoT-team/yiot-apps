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
#include <controllers/KSQBlankDevicesController.h>

#include <virgil/iot/qt/VSQIoTKit.h>

#include <yiot-iotkit/KSQIoTKitFacade.h>
#include <yiot-iotkit/setup/KSQDeviceSetupController.h>

//-----------------------------------------------------------------------------
KSQBlankDevicesController::KSQBlankDevicesController() {
    m_netif.clear();

    // SNAP::INFO service
    connect(&VSQSnapInfoClient::instance(),
            &VSQSnapInfoClient::fireNewDevice,
            this,
            &KSQBlankDevicesController::onDeviceInfoUpdate,
            Qt::QueuedConnection);

    connect(&VSQSnapInfoClient::instance(),
            &VSQSnapInfoClient::fireDeviceInfo,
            this,
            &KSQBlankDevicesController::onDeviceInfoUpdate,
            Qt::QueuedConnection);

    // Cleaner
    connect(&m_cleanerTimer, &QTimer::timeout, this, &KSQBlankDevicesController::cleanOldDevices);

    m_cleanerTimer.start(kInactiveTimeoutMS / 5);
}

//-----------------------------------------------------------------------------
bool
KSQBlankDevicesController::provisionDevice(const QString &mac) {
    return KSQDeviceSetupController::instance().start(m_netif, mac);
}

//-----------------------------------------------------------------------------
void
KSQBlankDevicesController::onDeviceInfoUpdate(const VSQDeviceInfo &deviceInfo) {
    if (deviceInfo.m_deviceName.isEmpty()) {
        return;
    }

    auto key = deviceInfo.m_mac.description();
    bool isNew = !m_devices.keys().contains(key);

    if (isNew) {
        auto tmp = m_devices;
        tmp[key] = BlankDevInfo();
        const int _pos = tmp.keys().indexOf(key);
        beginInsertRows(QModelIndex(), _pos, _pos);
    }

    m_devices[key].name = deviceInfo.m_deviceName;
    m_devices[key].type = "";
    m_devices[key].mac = deviceInfo.m_mac.description();
    m_devices[key].lastUpdate = QDateTime::currentDateTime();

    if (isNew) {
        endInsertRows();
        if (!deviceInfo.m_hasProvision) {
            emit fireDeviceRequiresProvision(deviceInfo.m_deviceName, m_netif, deviceInfo.m_mac);
        }
    } else {
        const auto _idx = createIndex(m_devices.keys().indexOf(key), 0);
        emit dataChanged(_idx, _idx);
    }
}

//-----------------------------------------------------------------------------
void
KSQBlankDevicesController::cleanOldDevices() {
    for (const auto &k : m_devices.keys()) {
        if (m_devices[k].lastUpdate.msecsTo(QDateTime::currentDateTime()) > kInactiveTimeoutMS) {
            const auto _pos = m_devices.keys().indexOf(k);
            beginRemoveRows(QModelIndex(), _pos, _pos);
            m_devices.remove(k);
            endRemoveRows();
        }
    }
}

//-----------------------------------------------------------------------------
int
KSQBlankDevicesController::rowCount(const QModelIndex &parent) const {
    return m_devices.count();
}

//-----------------------------------------------------------------------------
int
KSQBlankDevicesController::columnCount(const QModelIndex &parent) const {
    return 1;
}

//-----------------------------------------------------------------------------
QVariant
KSQBlankDevicesController::data(const QModelIndex &index, int role) const {
    if (index.row() < m_devices.count()) {
        auto key = m_devices.keys().at(index.row());

        switch (role) {
        case Element::Name:
            return m_devices[key].name;

        case Element::Type:
            return m_devices[key].type;

        case Element::Mac:
            return m_devices[key].mac.description();
        }
    }

    return QVariant();
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray>
KSQBlankDevicesController::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Type] = "type";
    roles[Mac] = "mac";
    return roles;
}

//-----------------------------------------------------------------------------
