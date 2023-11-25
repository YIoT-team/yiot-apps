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
#include <controllers/devices/KSQDevicesType.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapINFOClient.h>
#include <yiot-iotkit/snap/KSQSnapSCRTClient.h>

#if defined(Q_OS_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

//-----------------------------------------------------------------------------
KSQDevicesType::KSQDevicesType(QQmlApplicationEngine &engine, QSharedPointer<KSQOneExtension> device) {
    QString deviceDir(device->basePath());
    // Create JS processor
    const QString js = deviceDir + "/src/js/main.qml";
    QQmlComponent component(&engine, QUrl(js));
    if (component.isError()) {
        qDebug() << component.errors();
    }
    QObject *object = component.create();
    if (!object) {
        VS_LOG_ERROR("Cannot create QML processor for device type : %s", deviceDir.toStdString().c_str());
        return;
    }
    m_qmlProcessor.reset(object);

    // Add Device control page
    auto *rootObj = engine.rootObjects().first();
    auto deviceControlContainer = rootObj->findChild<QObject *>("deviceControlContainer");
    const QString controlPage = deviceDir + "/src/qml/Control.qml";
    QVariant res;
    if (QMetaObject::invokeMethod(deviceControlContainer,
                                  "addDeviceControl",
                                  Q_RETURN_ARG(QVariant, res),
                                  Q_ARG(QVariant, QVariant::fromValue(controlPage)))) {
        object->setProperty("controlPageIdx", res);
        device->setJs(object);
    } else {
        VS_LOG_ERROR("Cannot register device control page");
    }

    // Get device ID
    auto parts = deviceDir.split("/", Qt::SkipEmptyParts);
    if (parts.size()) {
        bool ok;
        auto val = parts.last().toInt(&ok);
        if (ok) {
            m_deviceId = val;
        } else {
            VS_LOG_ERROR("Cannot detect device ID");
        }
    }

    // SNAP::INFO service
    connect(&VSQSnapInfoClient::instance(),
            &VSQSnapInfoClient::fireNewDevice,
            this,
            &KSQDevicesType::onDeviceInfoUpdate,
            Qt::QueuedConnection);

    connect(&VSQSnapInfoClient::instance(),
            &VSQSnapInfoClient::fireNewDevice,
            this,
            &KSQDevicesType::fireNewUnknownDevice,
            Qt::QueuedConnection);

    connect(&VSQSnapInfoClient::instance(),
            &VSQSnapInfoClient::fireDeviceInfo,
            this,
            &KSQDevicesType::onDeviceInfoUpdate,
            Qt::QueuedConnection);

    // SNAP::USER service
    connect(&KSQSnapUSERClient::instance(),
            &KSQSnapUSERClient::fireStateUpdate,
            this,
            &KSQDevicesType::onDeviceStateUpdate,
            Qt::QueuedConnection);

    connect(&KSQSnapUSERClient::instance(),
            &KSQSnapUSERClient::fireStateError,
            this,
            &KSQDevicesType::onDeviceError,
            Qt::QueuedConnection);

    // SNAP:SCRT
    connect(&KSQSnapSCRTClient::instance(),
            &KSQSnapSCRTClient::fireSessionKeyReady,
            this,
            &KSQDevicesType::onSessionKeyReady,
            Qt::QueuedConnection);

    connect(&KSQSnapSCRTClient::instance(),
            &KSQSnapSCRTClient::fireSessionKeyError,
            this,
            &KSQDevicesType::onSessionKeyError,
            Qt::QueuedConnection);
}

//-----------------------------------------------------------------------------
void
KSQDevicesType::onSetDeviceName(VSQMac mac, QString name) {
    VSQSnapInfoClient::instance().onSetName(mac, name);
}

//-----------------------------------------------------------------------------
void
KSQDevicesType::onRequestSessionKey(VSQMac mac) {
    KSQSnapSCRTClient::instance().requestSessionKey(vs_snap_netif_routing(), mac);
}

//-----------------------------------------------------------------------------
void
KSQDevicesType::onSessionKeyReady(VSQMac mac, KSQSessionKey sessionKey) {
    qDebug() << "onSessionKeyReady: " << mac.description();

    auto res = findDevice(mac);
    auto device = res.second;
    if (device) {
        device->setSessionKey(sessionKey);
    }

    if (!m_active) {
        m_active = true;
        emit fireAboutToActivate();
        emit fireActivated();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevicesType::onSessionKeyError(VSQMac mac) {
    qDebug() << "onSessionKeyError: " << mac.description();
}

//-----------------------------------------------------------------------------
void
KSQDevicesType::onDeviceInfoUpdate(const struct VirgilIoTKit::vs_netif_t *src_netif, const VSQDeviceInfo &deviceInfo) {
    (void)src_netif;
    auto res = findDevice(deviceInfo.m_mac);
    auto device = res.second;
    if (device) {
        if (deviceInfo.m_hasGeneralInfo) {
            bool nameIsOld(!device->isUpdatedName());

            device->setName(deviceInfo.m_deviceName);
            device->setDeviceID(deviceInfo.m_deviceRoles);
            device->setManufacture(deviceInfo.m_manufactureId);
            device->setDeviceID(deviceInfo.m_deviceType);
            device->setFwVersion(deviceInfo.m_fwVer);
            device->setTlVersion(deviceInfo.m_tlVer);

            device->setHasProvision(deviceInfo.m_hasProvision);
            device->setHasOwner(deviceInfo.m_hasOwner);

            if (nameIsOld && device->isUpdatedName()) {
                emit fireRequiredSetup(device);
            }
        }

        if (deviceInfo.m_hasStatistics) {
            device->setSentBytes(QString("%1").arg(deviceInfo.m_sent));
            device->setReceivedBytes(QString("%1").arg(deviceInfo.m_received));
        }

        if (!deviceInfo.m_isActive && device->isWaitingReboot()) {
            VS_LOG_INFO("DROP SESSION DUE TO REBOOT: %s", VSQCString(deviceInfo.m_mac.description()));
            device->dropSession();
            removeDevice(deviceInfo.m_mac);
        }

        const auto _idx = createIndex(res.first, 0);
        emit dataChanged(_idx, _idx);
    }
}

//-----------------------------------------------------------------------------
void
KSQDevicesType::onDeviceStateUpdate(const vs_mac_addr_t mac, QString data) {
    Q_UNUSED(data)

    auto res = findDevice(mac);
    auto device = res.second;
    if (!device) {
        bool needAdd = false;
        try {
            QJsonDocument jsonState = QJsonDocument::fromJson(data.toUtf8());
            QJsonObject jsonObject = jsonState.object();
            int type = jsonObject["type"].toInt();

            if (static_cast<uint64_t>(type) == m_deviceId) {
                needAdd = true;
            }
        } catch (...) {
        }

        if (!needAdd) {
            return;
        }

        // Add Device
        beginInsertRows(QModelIndex(), m_devices.size(), m_devices.size());

        VSQMac qMac = VSQMac(mac);
        auto newDevice = QSharedPointer<KSQDevice>::create(m_qmlProcessor, qMac, qMac.description());
        connect(newDevice.get(), &KSQDevice::fireInvokeCommand, this, &KSQDevicesType::onInvokeCommand);
        connect(newDevice.get(), &KSQDevice::fireSetNameToHardware, this, &KSQDevicesType::onSetDeviceName);
        connect(newDevice.get(), &KSQDevice::fireRequestSessionKey, this, &KSQDevicesType::onRequestSessionKey);
        connect(newDevice.get(), &KSQDevice::fireSessionKeyReceived, this, &KSQDevicesType::fireSessionKeyReceived);

        m_devices.push_back(newDevice);

        endInsertRows();
    }

    res = findDevice(mac);
    device = res.second;
    if (device) {

        // Send JSON to JS processing
        QVariant varDevice;
        varDevice.setValue(device.get());
        if (!QMetaObject::invokeMethod(
                    m_qmlProcessor.get(), "onCommand", Q_ARG(QVariant, varDevice), Q_ARG(QVariant, data))) {
            VS_LOG_ERROR("Cannot process device command in JS");
        }

        device->commandDone();

        const auto _idx = createIndex(res.first, 0);
        emit dataChanged(_idx, _idx);
    }
}

//-----------------------------------------------------------------------------
void
KSQDevicesType::onDeviceError(const vs_mac_addr_t mac) {
    auto res = findDevice(mac);
    auto device = res.second;
    if (device) {
        qDebug() << "Device error: " << VSQMac(mac).description();
        device->commandError();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevicesType::onInvokeCommand(QString mac, QString json) {
    KSQSnapUSERClient::instance().sendCommand(mac, json);
}

//-----------------------------------------------------------------------------
std::pair<int, QSharedPointer<KSQDevice>>
KSQDevicesType::findDevice(const vs_mac_addr_t &mac) {
    VSQMac qMac(mac);
    int pos = 0;
    for (auto el : m_devices) {
        if (el->qMacAddr() == qMac) {
            return std::make_pair(pos, el);
        }
        ++pos;
    }
    return std::make_pair(-1, QSharedPointer<KSQDevice>(nullptr));
}

//-----------------------------------------------------------------------------
bool
KSQDevicesType::removeDevice(const vs_mac_addr_t &mac) {
    auto res = findDevice(mac);
    auto device = res.second;
    if (!device) {
        return true;
    }

    beginRemoveRows(QModelIndex(), res.first, res.first);

    device->disconnect();
    m_devices.remove(device);

    endRemoveRows();

    return true;
}

//-----------------------------------------------------------------------------
int
KSQDevicesType::rowCount(const QModelIndex &parent) const {
    return m_devices.size();
}

//-----------------------------------------------------------------------------
int
KSQDevicesType::columnCount(const QModelIndex &parent) const {
    return 1;
}

//-----------------------------------------------------------------------------
QVariant
KSQDevicesType::data(const QModelIndex &index, int role) const {
    if (index.row() < static_cast<int>(m_devices.size())) {
        auto l = *std::next(m_devices.begin(), index.row());

        switch (role) {
        case Element::Name:
            return l->name();

        case Element::Type:
            return l->deviceType();

        case Element::Mac:
            return l->macAddr();

        case Element::Active:
            return l->active();

        case Element::Secure:
            return l->hasSessionKey();

        case Element::Device: {
            QVariant res;
            res.setValue(const_cast<KSQDevice *>(&(*l)));
            return res;
        }


        case Element::Js: {
            QVariant resJs;
            resJs.setValue(m_qmlProcessor.get());
            return resJs;
        }
        }
    }

    return QVariant();
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray>
KSQDevicesType::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Type] = "deviceType";
    roles[Mac] = "mac";
    roles[Active] = "active";
    roles[Secure] = "secure";
    roles[Device] = "deviceController";
    roles[Js] = "js";
    return roles;
}

//-----------------------------------------------------------------------------
QSharedPointer<QObject>
KSQDevicesType::qmlProcessor() const {
    return m_qmlProcessor;
}

//-----------------------------------------------------------------------------
// TODO: remove from C++. Use in QML/JS only
QString
KSQDevicesType::name() const {
    static QString name;

    if (name.isEmpty()) {
        QVariant res;
        if (QMetaObject::invokeMethod(m_qmlProcessor.get(), "deviceName", Q_RETURN_ARG(QVariant, res))) {
            name = res.toString();
        } else {
            VS_LOG_ERROR("Cannot get device name");
        }
    }

    return name;
}

//-----------------------------------------------------------------------------
// TODO: remove from C++. Use in QML/JS only
QString
KSQDevicesType::type() const {
    static QString type;

    if (type.isEmpty()) {
        QVariant res;
        if (QMetaObject::invokeMethod(m_qmlProcessor.get(), "type", Q_RETURN_ARG(QVariant, res))) {
            type = res.toString();
        } else {
            VS_LOG_ERROR("Cannot get device type");
        }
    }

    return type;
}

//-----------------------------------------------------------------------------
// TODO: remove from C++. Use in QML/JS only
QString
KSQDevicesType::image() const {
    static QString image;

    if (image.isEmpty()) {
        QVariant res;
        if (QMetaObject::invokeMethod(m_qmlProcessor.get(), "image", Q_RETURN_ARG(QVariant, res))) {
            image = res.toString();
        } else {
            VS_LOG_ERROR("Cannot get device image");
        }
    }

    return image;
}

//-----------------------------------------------------------------------------
