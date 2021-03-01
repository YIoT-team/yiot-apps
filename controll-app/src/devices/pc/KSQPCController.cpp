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

#include <devices/pc/KSQPCController.h>

#include <virgil/iot/qt/protocols/snap/VSQSnapINFOClient.h>
#include <yiot-iotkit/snap/KSQSnapPCClient.h>
#include <yiot-iotkit/snap/KSQSnapSCRTClient.h>

#if defined(Q_OS_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

//-----------------------------------------------------------------------------
KSQPCController::KSQPCController() {
    // SNAP::INFO service
    connect(&VSQSnapInfoClient::instance(),
            &VSQSnapInfoClient::fireNewDevice,
            this,
            &KSQPCController::onDeviceInfoUpdate,
            Qt::QueuedConnection);

    connect(&VSQSnapInfoClient::instance(),
            &VSQSnapInfoClient::fireDeviceInfo,
            this,
            &KSQPCController::onDeviceInfoUpdate,
            Qt::QueuedConnection);

    // SNAP::PC service
    connect(&KSQSnapPCClient::instance(),
            &KSQSnapPCClient::fireStateUpdate,
            this,
            &KSQPCController::onPCStateUpdate,
            Qt::QueuedConnection);

    connect(&KSQSnapPCClient::instance(),
            &KSQSnapPCClient::fireStateError,
            this,
            &KSQPCController::onPCError,
            Qt::QueuedConnection);

    // SNAP:SCRT
    connect(&KSQSnapSCRTClient::instance(),
            &KSQSnapSCRTClient::fireSessionKeyReady,
            this,
            &KSQPCController::onSessionKeyReady,
            Qt::QueuedConnection);

    connect(&KSQSnapSCRTClient::instance(),
            &KSQSnapSCRTClient::fireSessionKeyError,
            this,
            &KSQPCController::onSessionKeyError,
            Qt::QueuedConnection);
}

//-----------------------------------------------------------------------------
void
KSQPCController::onSessionKeyReady(VSQMac mac, KSQSessionKey sessionKey) {
    qDebug() << "onSessionKeyReady: " << mac.description();

    auto res = findPC(mac);
    auto pc = res.second;
    if (pc) {
        pc->setSessionKey(sessionKey);
    }
}

//-----------------------------------------------------------------------------
void
KSQPCController::onSessionKeyError(VSQMac mac) {
    qDebug() << "onSessionKeyError: " << mac.description();
}

//-----------------------------------------------------------------------------
void
KSQPCController::onDeviceInfoUpdate(const VSQDeviceInfo &deviceInfo) {
    auto res = findPC(deviceInfo.m_mac);
    auto pc = res.second;
    if (pc) {
        if (deviceInfo.m_hasGeneralInfo) {
            pc->setDeviceID(deviceInfo.m_deviceRoles);
            pc->setManufacture(deviceInfo.m_manufactureId);
            pc->setDeviceID(deviceInfo.m_deviceType);
            pc->setFwVersion(deviceInfo.m_fwVer);
            pc->setTlVersion(deviceInfo.m_tlVer);

            pc->setHasProvision(deviceInfo.m_hasProvision);
            pc->setHasOwner(deviceInfo.m_hasOwner);
        }

        if (deviceInfo.m_hasStatistics) {
            pc->setSentBytes(QString("%1").arg(deviceInfo.m_sent));
            pc->setReceivedBytes(QString("%1").arg(deviceInfo.m_received));
        }

        const auto _idx = createIndex(res.first, 0);
        emit dataChanged(_idx, _idx);
    }
}

//-----------------------------------------------------------------------------
void
KSQPCController::onPCStateUpdate(const vs_mac_addr_t mac, const vs_snap_pc_state_t state) {
    auto res = findPC(mac);
    auto pc = res.second;
    bool added = false;
    if (!pc) {
        // Add PC

        bool activating = !m_pcs.size();

        if (activating) {
            emit fireAboutToActivate();
        }

        beginInsertRows(QModelIndex(), m_pcs.size(), m_pcs.size());

        auto newPC = QSharedPointer<KSQPC>::create(VSQMac(mac), QString("test-%1").arg(m_pcs.size()));
        connect(newPC.get(), &KSQPC::fireInvokeCommand, this, &KSQPCController::onInvokeCommand);
        connect(newPC.get(), &KSQPC::fireSetNameToHardware, this, &KSQControllerBase::onSetDeviceName);
        connect(newPC.get(), &KSQPC::fireRequestSessionKey, this, &KSQControllerBase::onRequestSessionKey);
        m_pcs.push_back(newPC);

        endInsertRows();

        if (activating) {
            emit fireActivated();
        }

        added = true;
    }

    res = findPC(mac);
    pc = res.second;
    if (pc) {
#if 0
            if (deviceInfo.m_hasGeneralInfo) {
                pc->setDeviceID(deviceInfo.m_deviceRoles);
                pc->setManufacture(deviceInfo.m_manufactureId);
                pc->setDeviceID(deviceInfo.m_deviceType);
                pc->setFwVersion(deviceInfo.m_fwVer);
                pc->setTlVersion(deviceInfo.m_tlVer);
            }

            if (deviceInfo.m_hasStatistics) {
                pc->setSentBytes(QString("%1").arg(deviceInfo.m_sent));
                pc->setReceivedBytes(QString("%1").arg(deviceInfo.m_received));
            }
#endif

        pc->commandDone();

        const auto _idx = createIndex(res.first, 0);
        emit dataChanged(_idx, _idx);

        if (added) {
            emit fireRequiredSetup(pc);
        }
    }
}

//-----------------------------------------------------------------------------
void
KSQPCController::onPCError(const vs_mac_addr_t mac) {
    auto res = findPC(mac);
    auto pc = res.second;
    if (pc) {
        qDebug() << "PC error: " << VSQMac(mac).description();
        pc->commandError();
    }
}

//-----------------------------------------------------------------------------
void
KSQPCController::onInvokeCommand(QString mac, QString json) {
    KSQSnapPCClient::instance().sendCommand(mac, json);
}

//-----------------------------------------------------------------------------
std::pair<int, QSharedPointer<KSQPC>>
KSQPCController::findPC(const vs_mac_addr_t &mac) {
    VSQMac qMac(mac);
    int pos = 0;
    for (auto el : m_pcs) {
        if (el->qMacAddr() == qMac) {
            return std::make_pair(pos, el);
        }
        ++pos;
    }
    return std::make_pair(-1, QSharedPointer<KSQPC>(nullptr));
}

//-----------------------------------------------------------------------------
int
KSQPCController::rowCount(const QModelIndex &parent) const {
    return m_pcs.size();
}

//-----------------------------------------------------------------------------
int
KSQPCController::columnCount(const QModelIndex &parent) const {
    return 1;
}

//-----------------------------------------------------------------------------
QVariant
KSQPCController::data(const QModelIndex &index, int role) const {
    if (index.row() < static_cast<int>(m_pcs.size())) {
        auto l = *std::next(m_pcs.begin(), index.row());

        switch (role) {
        case Element::Name:
            return l->name();

        case Element::Type:
            return l->deviceType();

        case Element::Mac:
            return l->macAddr();

        case Element::Active:
            return l->active();

        case Element::Device:
            QVariant res;
            res.setValue(const_cast<KSQPC *>(&(*l)));
            return res;
        }
    }

    return QVariant();
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray>
KSQPCController::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Type] = "deviceType";
    roles[Mac] = "mac";
    roles[Active] = "active";
    roles[Device] = "deviceController";
    return roles;
}

//-----------------------------------------------------------------------------
