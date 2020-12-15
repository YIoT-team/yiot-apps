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

#include <devices/KSQDeviceBase.h>

const QString KSQDeviceBase::kCmdStateConnect = "connect";
const QString KSQDeviceBase::kCmdStateSend = "send";
const QString KSQDeviceBase::kCmdStateReceive = "receive";
const QString KSQDeviceBase::kCmdStateDisconnect = "disconnect";
const QString KSQDeviceBase::kCmdStateDone = "done";
const QString KSQDeviceBase::kCmdStateError = "error";

//-----------------------------------------------------------------------------
KSQDeviceBase::KSQDeviceBase() {
}

//-----------------------------------------------------------------------------
KSQDeviceBase::KSQDeviceBase(VSQMac mac, QString name, QString img) {
    m_lastUpdate = QDateTime::currentDateTime();
    m_image = img;
    m_name = name;
    m_mac = mac;
    m_active = true;
}

//-----------------------------------------------------------------------------
KSQDeviceBase::KSQDeviceBase(const KSQDeviceBase &d) {
    m_lastUpdate = d.m_lastUpdate;
    m_image = d.m_image;
    m_name = d.m_name;
    m_mac = d.m_mac;
    m_active = d.m_active;
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::deviceType() const {
    return _deviceType();
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::macAddr() const {
    return m_mac;
}

//-----------------------------------------------------------------------------
VSQMac
KSQDeviceBase::qMacAddr() const {
    return m_mac;
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::setName(QString name) {
    if (name != m_name) {
        m_name = name;
        emit fireNameChanged();
    }
    emit fireSendNameUpdate();
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::setNameToHardware(QString name) {
    setName(name);
    emit fireSetNameToHardware(qMacAddr(), name);
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::setRoles(QString val) {
    if (val != m_roles) {
        m_roles = val;
        emit fireRolesChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::setManufacture(QString val) {
    if (val != m_manufacture) {
        m_manufacture = val;
        emit fireManufactureChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::setDeviceID(QString val) {
    if (val != m_deviceID) {
        m_deviceID = val;
        emit fireDeviceIDChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::setFwVersion(QString val) {
    if (val != m_fwVersion) {
        m_fwVersion = val;
        emit fireFwVersionChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::setTlVersion(QString val) {
    if (val != m_tlVer) {
        m_tlVer = val;
        emit fireTlVerChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::setSentBytes(QString val) {
    if (val != m_sentBytes) {
        m_sentBytes = val;
        emit fireSentBytesChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::setReceivedBytes(QString val) {
    if (val != m_receivedBytes) {
        m_receivedBytes = val;
        emit fireReceivedBytesChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::commandStart() {
    setCommandState(kCmdStateReceive);
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::commandError() {
    setCommandState(kCmdStateError);
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::commandDone() {
    setCommandState(kCmdStateDone);
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::name() const {
    return m_name;
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::roles() const {
    return m_roles;
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::manufacture() const {
    return m_manufacture;
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::deviceID() const {
    return m_deviceID;
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::fwVersion() const {
    return m_fwVersion;
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::tlVersion() const {
    return m_tlVer;
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::sentBytes() const {
    return m_sentBytes;
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::receivedBytes() const {
    return m_receivedBytes;
}

//-----------------------------------------------------------------------------
QString
KSQDeviceBase::commandState() const {
    return m_commandState;
}

//-----------------------------------------------------------------------------
bool
KSQDeviceBase::active() const {
    return m_active;
}

//-----------------------------------------------------------------------------
bool
KSQDeviceBase::operator<(const KSQDeviceBase &rhs) const {
    return m_name < rhs.m_name;
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::setCommandState(QString val) {
    if (val != m_commandState) {
        m_commandState = val;
        emit fireCommandStateChanged();
    }
}

//-----------------------------------------------------------------------------
void
KSQDeviceBase::_setRecivedName(QString name) {
    if (name != m_name) {
        m_name = name;
        emit fireNameChanged();
    }
}

//-----------------------------------------------------------------------------//-----------------------------------------------------------------------------
void
KSQDeviceBase::_setRecivedActivity(bool active) {
    if (active != m_active) {
        m_active = active;
        emit fireActiveChanged();
    }
}

//-----------------------------------------------------------------------------
