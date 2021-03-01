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

#include <yiot-iotkit/secmodule/KSQSessionKey.h>

//-----------------------------------------------------------------------------
KSQSessionKey::KSQSessionKey() {
    m_valid = false;
}

//-----------------------------------------------------------------------------
KSQSessionKey::KSQSessionKey(const vs_session_key_t &key) {
    set(key);
}

//-----------------------------------------------------------------------------
KSQSessionKey::KSQSessionKey(const KSQSessionKey &key) {
    if (this == &key) {
        return;
    }

    m_valid = key.m_valid;
    memcpy(&m_key, &key.m_key, sizeof(m_key));
}

//-----------------------------------------------------------------------------
KSQSessionKey &
KSQSessionKey::operator=(KSQSessionKey const &key) {
    if (this == &key) {
        return *this;
    }

    m_valid = key.m_valid;
    memcpy(&m_key, &key.m_key, sizeof(m_key));

    return *this;
}

//-----------------------------------------------------------------------------
bool
KSQSessionKey::isValid() const {
    return m_valid;
}

//-----------------------------------------------------------------------------
bool
KSQSessionKey::set(const vs_session_key_t &key) {
    memcpy(&m_key, &key, sizeof(key));
    m_valid = true;

    return m_valid;
}

//-----------------------------------------------------------------------------
