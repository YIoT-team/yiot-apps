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

#include <yiot-iotkit/setup/KSQDeviceSetupData.h>

//-----------------------------------------------------------------------------
KSQDeviceSetupData::KSQDeviceSetupData() : QObject() {
}

//-----------------------------------------------------------------------------
KSQDeviceSetupData::KSQDeviceSetupData(KSQDeviceSetupData const &d) {
    *this = d;
}

//-----------------------------------------------------------------------------
KSQDeviceSetupData &
KSQDeviceSetupData::operator=(KSQDeviceSetupData const &d) {
    if (this == &d) {
        return *this;
    }

    m_hasProvision = d.m_hasProvision;
    m_hasOwner = d.m_hasOwner;
    m_ownerIsYou = d.m_ownerIsYou;
    m_deviceInfo = d.m_deviceInfo;

    return *this;
}

//-----------------------------------------------------------------------------
bool
KSQDeviceSetupData::operator==(const KSQDeviceSetupData &d) const {
    if (m_hasProvision != d.m_hasProvision)
        return false;
    if (m_hasOwner != d.m_hasOwner)
        return false;
    if (m_ownerIsYou != d.m_ownerIsYou)
        return false;
    if (m_deviceInfo != d.m_deviceInfo)
        return false;

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQDeviceSetupData::operator!=(const KSQDeviceSetupData &d) const {
    return !this->operator==(d);
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupData::setHasProvision(bool hasProvision) {
    m_hasProvision = hasProvision;
    emit fireHasOwnerChanged();
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupData::setHasOwner(bool hasOwner) {
    m_hasOwner = hasOwner;
    emit fireHasOwnerChanged();
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupData::setOwnerIsYou(bool ownerIsYou) {
    m_ownerIsYou = ownerIsYou;
    emit fireOwnerIsYouChanged();
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupData::setPublicKey(const KSQPublicKey &publicKey) {
    m_publicKey = publicKey;
    emit firePublicKeyChanged();
}

//-----------------------------------------------------------------------------
QString
KSQDeviceSetupData::publicKeyInfo() const {
    return m_publicKey.description();
}

//-----------------------------------------------------------------------------
void
KSQDeviceSetupData::setDeviceInfo(const VSQDeviceInfo &deviceInfo) {
    m_deviceInfo = deviceInfo;
    emit fireNameChanged();
    emit fireManufacturerChanged();
    emit fireNameChanged();
    emit fireNeedCredsChanged();
}

//-----------------------------------------------------------------------------
QString
KSQDeviceSetupData::name() const {
    return m_deviceInfo.m_deviceName;
}

//-----------------------------------------------------------------------------
QString
KSQDeviceSetupData::manufacturer() const {
    return m_deviceInfo.m_manufactureId.description();
}

//-----------------------------------------------------------------------------
bool
KSQDeviceSetupData::needCreds() const {
    return m_deviceInfo.m_needConnectionCreds;
}

//-----------------------------------------------------------------------------
