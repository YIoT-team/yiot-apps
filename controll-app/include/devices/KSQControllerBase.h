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

#ifndef YIOT_DEVICE_CONTROLLER_BASE_H
#define YIOT_DEVICE_CONTROLLER_BASE_H

#include <QtCore>
#include <QHash>
#include <QAbstractTableModel>

#include <virgil/iot/qt/VSQIoTKit.h>
#include <devices/KSQDeviceBase.h>

class KSQControllerBase : public QAbstractTableModel {
    Q_OBJECT
    Q_PROPERTY(bool collapsed READ collapsed WRITE setCollapsed NOTIFY fireCollapsedChanged)
public:
    KSQControllerBase() = default;
    virtual ~KSQControllerBase() = default;

    virtual QString
    name() const = 0;

    virtual QString
    type() const = 0;

    virtual QString
    image() const = 0;

    Q_INVOKABLE void
    setCollapsed(bool c) {
        if (m_collapsed != c) {
            m_collapsed = c;
            emit fireCollapsedChanged(m_collapsed);
        }
    }

    Q_INVOKABLE bool
    collapsed() {
        return m_collapsed;
    }

public slots:
    void
    onSetDeviceName(VSQMac mac, QString name);

signals:
    void
    fireCollapsedChanged(bool);

    void
    fireAboutToActivate();

    void
    fireActivated();

    void
    fireRequiredSetup(QSharedPointer<KSQDeviceBase> device);

private:
    bool m_collapsed = true;
};

#endif // YIOT_DEVICE_CONTROLLER_BASE_H
