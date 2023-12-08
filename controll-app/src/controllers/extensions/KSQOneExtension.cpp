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
                                 const QStringList a_languages,
                                 bool a_isBuiltIn,
                                 const QString &a_path) {
    m_logo = a_logo;
    m_name = a_name;
    m_version = a_version;
    m_description = a_descr;
    m_isBuiltIn = a_isBuiltIn;
    m_path = a_path;
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
bool
KSQOneExtension::isBuiltIn() const {
    return m_isBuiltIn;
}

//-----------------------------------------------------------------------------
QString
KSQOneExtension::basePath() const {
    return m_path;
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
QObject *
KSQOneExtension::js() {
    return m_js;
}

//-----------------------------------------------------------------------------
void
KSQOneExtension::setJs(QObject *js) {
    m_js = js;
    emit fireJsChanged();
}

//-----------------------------------------------------------------------------
