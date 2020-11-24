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

#ifndef YIOT_PC_H
#define YIOT_PC_H

#include <QtCore>
#include <common/protocols/snap/pc/pc-structs.h>

#include <devices/KSQDeviceBase.h>

class KSQPCController;

class KSQPC : public KSQDeviceBase {
    Q_OBJECT
    friend KSQPCController;

public:
    KSQPC() : KSQDeviceBase() {
    }

    KSQPC(VSQMac mac, QString name, QString img = "");

    KSQPC(const KSQPC &l);

    virtual ~KSQPC() = default;

    virtual QString
    _deviceType() const final {
        return "pc";
    }

signals:
    void
    fireInitDevice(const KSQPC &pc);

public slots:
    Q_INVOKABLE void
    initDevice(QString user, QString password, QString staticIP);

private:
    QString m_user;
    QString m_password;
    QString m_staticIP;
};

Q_DECLARE_METATYPE(KSQPC)
Q_DECLARE_METATYPE(KSQPC *)

#endif // YIOT_PC_H
