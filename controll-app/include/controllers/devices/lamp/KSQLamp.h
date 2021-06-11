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

#ifndef YIOT_LAMP_H
#define YIOT_LAMP_H


#include <QtCore>
#include <common/protocols/snap/lamp/lamp-structs.h>

#include <controllers/devices/KSQDeviceBase.h>

class KSQLamp : public KSQDeviceBase {
    Q_OBJECT
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY fireStateChanged)
public:
    KSQLamp() : KSQDeviceBase() {
    }
    KSQLamp(VSQMac mac, QString name, QString img = "");
    KSQLamp(const KSQLamp &l);
    virtual ~KSQLamp() = default;

    Q_INVOKABLE QString
    state() const;

    virtual QString
    _deviceType() const final {
        return "lampMono";
    }

    static const QString kStateUnknown;
    static const QString kStateOn;
    static const QString kStateOff;
    static const QStringList kStates;

signals:
    void fireStateChanged(QString);

    void
    fireSetDeviceParams(const KSQLamp &lamp);

public slots:
    Q_INVOKABLE void
    setState(QString state);

    Q_INVOKABLE void
    setStateToHardware(QString state);

private:
    QString m_state;
};

Q_DECLARE_METATYPE(KSQLamp)
Q_DECLARE_METATYPE(KSQLamp *)

#endif // YIOT_LAMP_H
