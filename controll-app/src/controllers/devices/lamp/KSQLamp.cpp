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

#include <controllers/devices/lamp/KSQLampController.h>

const QString KSQLamp::kStateUnknown = "unknown";
const QString KSQLamp::kStateOn = "on";
const QString KSQLamp::kStateOff = "off";
const QStringList KSQLamp::kStates = QStringList() << KSQLamp::kStateUnknown << KSQLamp::kStateOn << KSQLamp::kStateOff;

//-----------------------------------------------------------------------------
KSQLamp::KSQLamp(VSQMac mac, QString name, QString img) : KSQDeviceBase(mac, name, img) {
    m_state = kStateUnknown;
}

//-----------------------------------------------------------------------------
KSQLamp::KSQLamp(const KSQLamp &l) : KSQDeviceBase(l) {
    m_state = l.m_state;
}

//-----------------------------------------------------------------------------
QString
KSQLamp::state() const {
    return m_state;
}

//-----------------------------------------------------------------------------
void
KSQLamp::setState(QString state) {
    if (kStates.contains(state)) {
        m_state = state;
    } else {
        m_state = kStateUnknown;
    }
    emit fireStateChanged(m_state);
    emit fireDeviceStateChanged();
}

//-----------------------------------------------------------------------------
Q_INVOKABLE void
KSQLamp::setStateToHardware(QString state) {
    setState(state);
    emit fireSetDeviceParams(*this);
}

//-----------------------------------------------------------------------------
