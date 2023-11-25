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

#include <controllers/devices/KSQAllDevicesController.h>
#include <yiot-iotkit/snap/KSQSnapSCRTClient.h>

//-----------------------------------------------------------------------------
void
KSQAllDevicesController::add(KSQDevicesType *devicesType) {
    connect(devicesType, &KSQDevicesType::fireActivated, this, &KSQAllDevicesController::onGroupActivated);
    connect(devicesType, &KSQDevicesType::fireRequiredSetup, this, &KSQAllDevicesController::fireNewProvisionedDevice);
    connect(devicesType, &KSQDevicesType::fireNewUnknownDevice, this, &KSQAllDevicesController::fireNewUnknownDevice);
    connect(devicesType,
            &KSQDevicesType::fireSessionKeyReceived,
            this,
            &KSQAllDevicesController::fireSessionKeyReceived);
    QSharedPointer<KSQDevicesType> e(devicesType);
    m_elements.push_back(e);
}

//-----------------------------------------------------------------------------
bool
KSQAllDevicesController::load(QSharedPointer<KSQOneExtension> extension) {
    add(new KSQDevicesType(*m_engine, extension));
    return true;
}

//-----------------------------------------------------------------------------
void
KSQAllDevicesController::onGroupActivated() {
    // TODO: Use correct insertion
    beginResetModel();
    endResetModel();
}

//-----------------------------------------------------------------------------
int
KSQAllDevicesController::rowCount(const QModelIndex &parent) const {
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
KSQAllDevicesController::columnCount(const QModelIndex &parent) const {
    return 1;
}

//-----------------------------------------------------------------------------
QVariant
KSQAllDevicesController::data(const QModelIndex &index, int role) const {
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

                case Element::SubModel: {
                    QVariant res;
                    res.setValue(el.get());
                    return res;
                }

                case Element::Js: {
                    QVariant res;
                    res.setValue(el->qmlProcessor().get());
                    return res;
                }
                }
            }
            ++cnt;
        }
    }
    return QVariant();
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray>
KSQAllDevicesController::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Type] = "categoryType";
    roles[Image] = "image";
    roles[SubModel] = "subModel";
    roles[Js] = "js";
    return roles;
}

//-----------------------------------------------------------------------------
