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
    m_name = "TestName";
    m_manufacturer = "YIoT";

    m_hasProvision = false;
    m_hasOwner = false;
    m_ownerIsYou = false;
    m_needCreds = true;
}

//-----------------------------------------------------------------------------
KSQDeviceSetupData::KSQDeviceSetupData(KSQDeviceSetupData const &d) {
    *this = d;
}

//-----------------------------------------------------------------------------
KSQDeviceSetupData &
KSQDeviceSetupData::operator=(KSQDeviceSetupData const & d) {
    if (this == &d) {
        return *this;
    }

    m_name = d.m_name;
    m_manufacturer = d.m_manufacturer;

    m_hasProvision = d.m_hasProvision;
    m_hasOwner = d.m_hasOwner;
    m_ownerIsYou = d.m_ownerIsYou;
    m_needCreds = d.m_needCreds;

    return *this;
}

//-----------------------------------------------------------------------------
bool
KSQDeviceSetupData::operator==(const KSQDeviceSetupData &d) const {
    if (m_name != d.m_name) return false;
    if (m_manufacturer != d.m_manufacturer) return false;

    if (m_hasProvision != d.m_hasProvision) return false;
    if (m_hasOwner != d.m_hasOwner) return false;
    if (m_ownerIsYou != d.m_ownerIsYou) return false;
    if (m_needCreds != d.m_needCreds) return false;

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQDeviceSetupData::operator!=(const KSQDeviceSetupData &d) const {
    return !this->operator==(d);
}

//-----------------------------------------------------------------------------
