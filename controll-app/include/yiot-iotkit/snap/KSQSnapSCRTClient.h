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

#ifndef _YIOT_QT_SNAP_SCRT_CLIENT_SERVICE_H_
#define _YIOT_QT_SNAP_SCRT_CLIENT_SERVICE_H_

#include <QtCore>

#include <virgil/iot/protocols/snap/scrt/scrt-structs.h>
#include <virgil/iot/protocols/snap/scrt/scrt-client.h>
#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapServiceBase.h>

using namespace VirgilIoTKit;

class KSQSnapSCRTClient final : public QObject, public VSQSingleton<KSQSnapSCRTClient>, public VSQSnapServiceBase {

    Q_OBJECT

    friend VSQSingleton<KSQSnapSCRTClient>;

public:
    const VirgilIoTKit::vs_snap_service_t *
    serviceInterface() override {
        return m_snapService;
    }

    const QString &
    serviceName() const override {
        static QString name{"SCRT Client"};
        return name;
    }

signals:


public slots:

private:
    const VirgilIoTKit::vs_snap_service_t *m_snapService;

    KSQSnapSCRTClient();
    virtual ~KSQSnapSCRTClient() = default;
};

#endif // _YIOT_QT_SNAP_SCRT_CLIENT_SERVICE_H_
