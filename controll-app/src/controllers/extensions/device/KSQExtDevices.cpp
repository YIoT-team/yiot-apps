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

#include <controllers/extensions/device/KSQExtDevices.h>

//-----------------------------------------------------------------------------
KSQExtDevices::KSQExtDevices() {
    qRegisterMetaType<KSQExtDevice>("KSQExtDevice");

    loadBuiltinDevicesInfo();
}

//-----------------------------------------------------------------------------
bool
KSQExtDevices::loadBuiltinDevicesInfo() {
    QString logo = "https://www.raspberrypi.org/app/uploads/2011/10/Raspi-PGB001.png";
    QString name = "Raspberry Pi";
    QString version = "0.0.1";
    QString description = "Ta da :)";

    m_devices << QSharedPointer<KSQExtDevice>::create(logo, name, version, description);
    return true;
}

//-----------------------------------------------------------------------------
int
KSQExtDevices::rowCount(const QModelIndex &parent) const {
    return m_devices.size();
}

//-----------------------------------------------------------------------------
int
KSQExtDevices::columnCount(const QModelIndex &parent) const {
    return 1;
}

//-----------------------------------------------------------------------------
QVariant
KSQExtDevices::data(const QModelIndex &index, int role) const {
    if (index.row() < m_devices.count()) {

        switch (role) {
        case Element::Info:
            QVariant res;
            res.setValue(m_devices.at(index.row()).get());
            return res;
        }
    }

    return QVariant();
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray>
KSQExtDevices::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Info] = "info";
    return roles;
}

//-----------------------------------------------------------------------------
