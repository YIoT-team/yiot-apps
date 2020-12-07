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

#ifndef _YIOT_QT_SECURITY_MODULE_H_
#define _YIOT_QT_SECURITY_MODULE_H_

#include <QtCore>

#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/secmodule/secmodule.h>

#include <yiot-iotkit/secmodule/KSQSecModule.h>
#include <yiot-iotkit/secmodule/KSQKeyPair.h>

using namespace VirgilIoTKit;

class KSQSecModule : public QObject, public VSQSingleton<KSQSecModule> {

    Q_OBJECT

    friend VSQSingleton<KSQSecModule>;

public:
    VirgilIoTKit::vs_secmodule_impl_t *
    secmoduleImpl() {
        return m_secmoduleImpl;
    }

    KSQKeyPair
    generateKeypair(vs_secmodule_keypair_type_e keypair_type) const;

signals:

public slots:

private:
    vs_secmodule_impl_t *m_secmoduleImpl;

    KSQSecModule();
    virtual ~KSQSecModule() = default;
};

#endif // _YIOT_QT_SECURITY_MODULE_H_
