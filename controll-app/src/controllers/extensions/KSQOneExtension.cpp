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

#include <controllers/extensions/KSQOneExtension.h>

//-----------------------------------------------------------------------------
KSQOneExtension::KSQOneExtension(const QString &a_logo,
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

KSQOneExtension::KSQOneExtension(KSQOneExtension const &) {
}

//-----------------------------------------------------------------------------
KSQOneExtension &
KSQOneExtension::operator=(KSQOneExtension const &) {
    return *this;
}

//-----------------------------------------------------------------------------
QString
KSQOneExtension::logo() const {
    return m_logo;
}

//-----------------------------------------------------------------------------
QString
KSQOneExtension::name() const {
    return m_name;
}

//-----------------------------------------------------------------------------
QString
KSQOneExtension::description() const {
    return m_description;
}

//-----------------------------------------------------------------------------
QString
KSQOneExtension::version() const {
    return m_version;
}

//-----------------------------------------------------------------------------
size_t
KSQOneExtension::size() const {
    return m_size;
}

//-----------------------------------------------------------------------------
QStringList
KSQOneExtension::lanuages() const {
    return m_languages;
}

//-----------------------------------------------------------------------------
QString
KSQOneExtension::link() const {
    return m_link;
}

//-----------------------------------------------------------------------------
