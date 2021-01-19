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

#if defined(Q_OS_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

//-----------------------------------------------------------------------------
#if defined(Q_OS_WIN32)
static int
inet_pton(int af, const char *src, void *dst) {
    struct sockaddr_storage ss;
    int size = sizeof(ss);
    char src_copy[INET6_ADDRSTRLEN + 1];

    ZeroMemory(&ss, sizeof(ss));
    /* stupid non-const API */
    strncpy(src_copy, src, INET6_ADDRSTRLEN + 1);
    src_copy[INET6_ADDRSTRLEN] = 0;

    if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
        switch (af) {
        case AF_INET:
            *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
            return 1;
        case AF_INET6:
            *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
            return 1;
        }
    }
    return 0;
}
#endif // Q_OS_WIN32
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
        connect(newPC.get(), &KSQPC::fireInitDevice, this, &KSQPCController::onInitDevice);
        connect(newPC.get(), &KSQPC::fireSetNameToHardware, this, &KSQControllerBase::onSetDeviceName);
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
KSQPCController::onInitDevice(KSQPC &pc) {
    //    vs_snap_pc_init_ssh_t init;
    //    struct in_addr addr;
    //
    //    memset(&init, 0, sizeof(init));
    //    memset(&addr, 0, sizeof(addr));
    //
    //    bool isOk = true;
    //    if ((pc.m_user.length() + 1) >= USER_NAME_SZ_MAX || (pc.m_password.length() + 1) >= USER_PASS_SZ_MAX) {
    //        isOk = false;
    //    }
    //
    //    if (isOk) {
    //        strcpy(reinterpret_cast<char *>(init.user), pc.m_user.toStdString().c_str());
    //        strcpy(reinterpret_cast<char *>(init.pass), pc.m_password.toStdString().c_str());
    //        if (0 >= inet_pton(AF_INET, pc.m_staticIP.toStdString().c_str(), &addr)) {
    //            isOk = false;
    //        } else {
    //#if defined(Q_OS_WIN32)
    //            init.ipv4 = addr.S_un.S_addr;
    //#else
    //            init.ipv4 = addr.s_addr;
    //#endif
    //        }
    //    }
    //
    //    if (!isOk) {
    //        VS_LOG_ERROR("Wrong parameters");
    //        pc.commandError();
    //        return;
    //    }
    //
    //    KSQSnapPCClient::instance().initPC(pc.qMacAddr(), init);
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
