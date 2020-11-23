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

#include <devices/KSQDevices.h>

void
fireAboutToActivate();
void
fireActivated();

//-----------------------------------------------------------------------------
KSQDevices &
KSQDevices::operator<<(KSQControllerBase *controller) {
    connect(controller, &KSQControllerBase::fireActivated, this, &KSQDevices::onGroupActivated);
    QSharedPointer<KSQControllerBase> e(controller);
    m_elements.push_back(e);
    return *this;
}

//-----------------------------------------------------------------------------
void
KSQDevices::onGroupActivated() {
    // TODO: Use correct insertion
    beginResetModel();
    endResetModel();
}

//-----------------------------------------------------------------------------
int
KSQDevices::rowCount(const QModelIndex &parent) const {
    int cnt = 0;
    for (const auto &el : m_elements) {
        if (el->rowCount()) {
            ++cnt;
        }
    }
    return cnt;
}

//-----------------------------------------------------------------------------
int
KSQDevices::columnCount(const QModelIndex &parent) const {
    return 1;
}

//-----------------------------------------------------------------------------
QVariant
KSQDevices::data(const QModelIndex &index, int role) const {
    int cnt = 0;
    for (const auto &el : m_elements) {
        if (el->rowCount()) {
            if (index.row() == cnt) {
                switch (role) {
                case Element::Name:
                    return el->name();

                case Element::Type:
                    return el->type();

                case Element::Image:
                    return el->image();

                case Element::SubModel:
                    QVariant res;
                    res.setValue(el.get());
                    return res;
                }
            }
            ++cnt;
        }
    }
    return cnt;
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray>
KSQDevices::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Type] = "categoryType";
    roles[Image] = "image";
    roles[SubModel] = "subModel";
    return roles;
}

//-----------------------------------------------------------------------------
