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

#include <QtCore>
#include <yiot-iotkit/root-of-trust/KSQRoTController.h>
#include <yiot-iotkit/secbox/KSQSecBox.h>

//-----------------------------------------------------------------------------
KSQRoTController::KSQRoTController() {
    memset(&m_listId, 0, sizeof(m_listId));
    strcpy(reinterpret_cast<char *>(m_listId), "rot_list");
    prepare();
}

//-----------------------------------------------------------------------------
int
KSQRoTController::rowCount(const QModelIndex &parent) const {
    return m_rots.size();
}

//-----------------------------------------------------------------------------
int
KSQRoTController::columnCount(const QModelIndex &parent) const {
    return 1;
}

//-----------------------------------------------------------------------------
QVariant
KSQRoTController::data(const QModelIndex &index, int role) const {
    if (index.row() < static_cast<int>(m_rots.size())) {
        auto r = *std::next(m_rots.begin(), index.row());

        switch (role) {
        case Element::ID:
            return r->id();
        case Element::Name:
            return r->name();
        case Element::Image:
            return r->image();
        case Element::TrustList: {
            QVariant res;
            res.setValue(const_cast<KSQTrustList *>(&r->trustList()));
            return res;
        }
        case Element::ECType:
            return r->ecType();
        case Element::Recovery1:
            return r->recovery1().description();
        case Element::Recovery2:
            return r->recovery2().description();
        case Element::Auth1:
            return r->auth1().description();
        case Element::Auth2:
            return r->auth2().description();
        case Element::TL1:
            return r->tl1().description();
        case Element::TL2:
            return r->tl2().description();
        case Element::Firmware1:
            return r->firmware1().description();
        case Element::Firmware2:
            return r->firmware2().description();
        default: {
        }
        }
    }

    return QVariant();
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray>
KSQRoTController::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ID] = "id";
    roles[Name] = "name";
    roles[Image] = "image";
    roles[TrustList] = "trustList";
    roles[ECType] = "ecType";
    roles[Recovery1] = "recovery1";
    roles[Recovery2] = "recovery2";
    roles[Auth1] = "auth1";
    roles[Auth2] = "auth2";
    roles[TL1] = "tl1";
    roles[TL2] = "tl2";
    roles[Firmware1] = "firmware1";
    roles[Firmware2] = "firmware2";

    return roles;
}

//-----------------------------------------------------------------------------
bool
KSQRoTController::prepare() {
    auto idsList = loadRoTList();

    if (!idsList.contains(KSQRoT::kLocalID)) {
        auto localRoT = QSharedPointer<KSQRoT>::create(KSQRoT::kLocalID, "secure-enclave");
        if (!localRoT->isValid()) {
            return false;
        }
        idsList << KSQRoT::kLocalID;

        m_isValid = saveRoTList(idsList);
        return m_isValid;
    }

    m_isValid = true;

    return m_isValid;
}

//-----------------------------------------------------------------------------
QStringList
KSQRoTController::loadRoTList() {
    return QStringList();
}

//-----------------------------------------------------------------------------
bool
KSQRoTController::saveRoTList(const QStringList& list) {
    QByteArray data;
    foreach (const auto &str, list) {
        data += str.toUtf8();
    }

    return KSQSecBox::instance().save(VS_SECBOX_SIGNED,
                                          m_listId,
                                          data);
}

//-----------------------------------------------------------------------------
