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

#include <arpa/inet.h>

#include <devices/pc/KSQPCController.h>

#include <virgil/iot/qt/protocols/snap/VSQSnapINFOClient.h>
#include <yiot-iotkit/snap/KSQSnapPCClient.h>

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

    // Test data
#if 1
    auto t = new QTimer();
    connect(t, &QTimer::timeout, [t, this]() {
        static int n = 0;
        if (n++ >= 10) {
            t->deleteLater();
            return;
        }

        vs_mac_addr_t mac;
        mac.bytes[0] = 32;
        mac.bytes[1] = 1;
        mac.bytes[2] = 2;
        mac.bytes[3] = 3;
        mac.bytes[4] = 4;
        mac.bytes[5] = n;

        VSQDeviceInfo deviceInfo;
        deviceInfo.m_mac = mac;
        deviceInfo.m_hasGeneralInfo = true;
        deviceInfo.m_deviceType = VSQDeviceType();
        deviceInfo.m_deviceRoles = VSQDeviceRoles() << VS_SNAP_DEV_THING;
        deviceInfo.m_tlVer = VSQFileVersion();
        deviceInfo.m_fwVer = VSQFileVersion();
        deviceInfo.m_manufactureId = VSQManufactureId();
        deviceInfo.m_hasStatistics = true;
        deviceInfo.m_sent = 123134;
        deviceInfo.m_received = 456234;
        onDeviceInfoUpdate(deviceInfo);
    });
    t->start(1000);
#endif
    // ~Test data
}

//-----------------------------------------------------------------------------
void
KSQPCController::onDeviceInfoUpdate(const VSQDeviceInfo &deviceInfo) {
    auto res = findPC(deviceInfo.m_mac);
    auto pc = res.second;
    if (!pc) {
        // Add PC

        bool activating = !m_pcs.size();

        if (activating) {
            emit fireAboutToActivate();
        }

        beginInsertRows(QModelIndex(), m_pcs.size(), m_pcs.size());

        auto newPC = QSharedPointer<KSQPC>::create(VSQMac(deviceInfo.m_mac), QString("test-%1").arg(m_pcs.size()));
        connect(newPC.get(), &KSQPC::fireInitDevice, this, &KSQPCController::onInitDevice);
        m_pcs.push_back(newPC);

        endInsertRows();

        if (activating) {
            emit fireActivated();
        }
    }

    res = findPC(deviceInfo.m_mac);
    pc = res.second;
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
KSQPCController::onInitDevice(const KSQPC &pc) {
    vs_snap_pc_init_t init;

    memset(&init, 0, sizeof(init));

    bool isOk = true;
    if ((pc.m_user.length() + 1) >= USER_NAME_SZ_MAX
        || (pc.m_password.length() + 1) >= USER_PASS_SZ_MAX) {
        isOk = false;
    }

    if (isOk) {
        strcpy(reinterpret_cast<char *>(init.user), pc.m_user.toStdString().c_str());
        strcpy(reinterpret_cast<char *>(init.pass), pc.m_password.toStdString().c_str());
        if (0 >= inet_pton(AF_INET, pc.m_staticIP.toStdString().c_str(), &init.ipv4)) {
            isOk = false;
        }
    }

    if (!isOk) {
        VS_LOG_ERROR("Wrong parameters");
        return;
    }

    KSQSnapPCClient::instance().initPC(pc.qMacAddr(), init);
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
