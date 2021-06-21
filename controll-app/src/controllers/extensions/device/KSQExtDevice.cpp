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

#include <controllers/extensions/device/KSQExtDevice.h>

//-----------------------------------------------------------------------------
KSQExtDevice::KSQExtDevice(const QString &a_logo,
                           const QString &a_name,
                           const QString &a_version,
                           const QString &a_descr,
                           const QString &a_link,
                           size_t a_size,
                           const QStringList a_languages) {
    m_logo = a_logo;
    m_name = a_name;
    m_version = a_version;
    m_description = a_descr;
}
//-----------------------------------------------------------------------------

KSQExtDevice::KSQExtDevice(KSQExtDevice const &) {
}

//-----------------------------------------------------------------------------
KSQExtDevice &
KSQExtDevice::operator=(KSQExtDevice const &) {
    return *this;
}

//-----------------------------------------------------------------------------
QString
KSQExtDevice::logo() const {
    return m_logo;
}

//-----------------------------------------------------------------------------
QString
KSQExtDevice::name() const {
    return m_name;
}

//-----------------------------------------------------------------------------
QString
KSQExtDevice::description() const {
    return m_description;
}

//-----------------------------------------------------------------------------
QString
KSQExtDevice::version() const {
    return m_version;
}

//-----------------------------------------------------------------------------
size_t
KSQExtDevice::size() const {
    return m_size;
}

//-----------------------------------------------------------------------------
QStringList
KSQExtDevice::lanuages() const {
    return m_languages;
}

//-----------------------------------------------------------------------------
QString
KSQExtDevice::link() const {
    return m_link;
}

//-----------------------------------------------------------------------------
