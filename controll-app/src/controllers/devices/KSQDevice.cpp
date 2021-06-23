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

#include <controllers/devices/KSQDevice.h>

const QString KSQDevice::kCmdStateConnect = "connect";
const QString KSQDevice::kCmdStateSend = "send";
const QString KSQDevice::kCmdStateReceive = "receive";
const QString KSQDevice::kCmdStateDisconnect = "disconnect";
const QString KSQDevice::kCmdStateDone = "done";
const QString KSQDevice::kCmdStateError = "error";

//-----------------------------------------------------------------------------
KSQDevice::KSQDevice() : m_nameUpdated(false) {
}

//-----------------------------------------------------------------------------
KSQDevice::KSQDevice(QSharedPointer<QObject> js, VSQMac mac, QString name, QString img) {
    m_lastUpdate = QDateTime::currentDateTime();
    m_image = img;
    m_name = name;
    m_nameUpdated = false;
    m_mac = mac;
    m_active = true;
    m_hasProvision = false;
    m_hasOwner = false;
    m_js = js;

    startSessionConnection();
}

//-----------------------------------------------------------------------------
KSQDevice::KSQDevice(const KSQDevice &d) {
    m_lastUpdate = d.m_lastUpdate;
    m_image = d.m_image;
    m_name = d.m_name;
    m_nameUpdated = d.m_nameUpdated;
    m_mac = d.m_mac;
    m_active = d.m_active;
    m_sessionKey = d.m_sessionKey;
    m_hasProvision = d.m_hasProvision;
    m_hasOwner = d.m_hasOwner;
    m_js = d.m_js;

    startSessionConnection();
}

//-----------------------------------------------------------------------------
bool
KSQDevice::isUpdatedName() {
    return m_nameUpdated;
}

//-----------------------------------------------------------------------------
void
KSQDevice::startSessionConnection() {
    if (m_sessionTimer.isActive()) {
        return;
    }

    m_sessionTimer.setSingleShot(false);
    m_sessionTimer.setInterval(kSessionCheckPeriodMs);
    connect(&m_sessionTimer, &QTimer::timeout, this, &KSQDevice::onSessionTimer);
    m_sessionTimer.start();
}

//-----------------------------------------------------------------------------
void
KSQDevice::onSessionTimer() {
    if (!m_hasProvision || m_sessionKey.isValid()) {
        return;
    }

    emit fireRequestSessionKey(m_mac);
}

//-----------------------------------------------------------------------------
QString
KSQDevice::deviceType() const {
    return _deviceType();
}

//-----------------------------------------------------------------------------
QString
KSQDevice::macAddr() const {
    return m_mac;
}

//-----------------------------------------------------------------------------
VSQMac
KSQDevice::qMacAddr() const {
    return m_mac;
}

//-----------------------------------------------------------------------------
void
KSQDevice::setName(QString name) {
    if (name != m_name) {
        m_name = name;
        emit fireNameChanged();
    }
    emit fireSendNameUpdate();
    m_nameUpdated = true;
}

//-----------------------------------------------------------------------------
void
KSQDevice::setNameToHardware(QString name) {
    setName(name);
    emit fireSetNameToHardware(qMacAddr(), name);
}

//-----------------------------------------------------------------------------
void
KSQDevice::setRoles(QString val) {
    if (val != m_roles) {
        m_roles = val;
        emit fireRolesChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::setManufacture(QString val) {
    if (val != m_manufacture) {
        m_manufacture = val;
        emit fireManufactureChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::setDeviceID(QString val) {
    if (val != m_deviceID) {
        m_deviceID = val;
        emit fireDeviceIDChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::setFwVersion(QString val) {
    if (val != m_fwVersion) {
        m_fwVersion = val;
        emit fireFwVersionChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::setTlVersion(QString val) {
    if (val != m_tlVer) {
        m_tlVer = val;
        emit fireTlVerChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::setSentBytes(QString val) {
    if (val != m_sentBytes) {
        m_sentBytes = val;
        emit fireSentBytesChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::setReceivedBytes(QString val) {
    if (val != m_receivedBytes) {
        m_receivedBytes = val;
        emit fireReceivedBytesChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::setHasOwner(bool hasOwner) {
    if (hasOwner != m_hasOwner) {
        m_hasOwner = hasOwner;
        emit fireHasOwnerChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::setHasProvision(bool hasProvision) {
    if (hasProvision != m_hasProvision) {
        m_hasProvision = hasProvision;
        emit fireHasProvisionChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::setSessionKey(const KSQSessionKey &key) {
    m_sessionKey = key;
    m_sessionTimer.stop();
    emit fireHasSessionKeyChanged();
}

//-----------------------------------------------------------------------------
void
KSQDevice::commandStart() {
    setCommandState(kCmdStateReceive);
}

//-----------------------------------------------------------------------------
void
KSQDevice::commandError() {
    setCommandState(kCmdStateError);
}

//-----------------------------------------------------------------------------
void
KSQDevice::commandDone() {
    setCommandState(kCmdStateDone);
}

//-----------------------------------------------------------------------------
QString
KSQDevice::name() const {
    return m_name;
}

//-----------------------------------------------------------------------------
QString
KSQDevice::roles() const {
    return m_roles;
}

//-----------------------------------------------------------------------------
QString
KSQDevice::manufacture() const {
    return m_manufacture;
}

//-----------------------------------------------------------------------------
QString
KSQDevice::deviceID() const {
    return m_deviceID;
}

//-----------------------------------------------------------------------------
QString
KSQDevice::fwVersion() const {
    return m_fwVersion;
}

//-----------------------------------------------------------------------------
QString
KSQDevice::tlVersion() const {
    return m_tlVer;
}

//-----------------------------------------------------------------------------
QString
KSQDevice::sentBytes() const {
    return m_sentBytes;
}

//-----------------------------------------------------------------------------
QString
KSQDevice::receivedBytes() const {
    return m_receivedBytes;
}

//-----------------------------------------------------------------------------
QString
KSQDevice::commandState() const {
    return m_commandState;
}

//-----------------------------------------------------------------------------
bool
KSQDevice::hasOwner() const {
    return m_hasOwner;
}

//-----------------------------------------------------------------------------
bool
KSQDevice::hasProvision() const {
    return m_hasProvision;
}

//-----------------------------------------------------------------------------
bool
KSQDevice::hasSessionKey() const {
    return m_sessionKey.isValid();
}

//-----------------------------------------------------------------------------
bool
KSQDevice::active() const {
    return m_active;
}

//-----------------------------------------------------------------------------
bool
KSQDevice::operator<(const KSQDevice &rhs) const {
    return m_name < rhs.m_name;
}

//-----------------------------------------------------------------------------
void
KSQDevice::setCommandState(QString val) {
    if (val != m_commandState) {
        m_commandState = val;
        emit fireCommandStateChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::_setRecivedName(QString name) {
    if (name != m_name) {
        m_name = name;
        emit fireNameChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDevice::_setRecivedActivity(bool active) {
    if (active != m_active) {
        m_active = active;
        emit fireActiveChanged();
    }
}

//-----------------------------------------------------------------------------
QObject *
KSQDevice::js() const {
    return m_js.get();
}

//-----------------------------------------------------------------------------
void
KSQDevice::invokeCommand(QString json) {
    commandStart();
    emit fireInvokeCommand(macAddr(), json);
}

//-----------------------------------------------------------------------------
