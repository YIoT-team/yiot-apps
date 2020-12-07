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

#ifndef _YIOT_QT_SECURITY_BOX_H_
#define _YIOT_QT_SECURITY_BOX_H_

#include <QtCore>

#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/secbox/secbox.h>

#include <yiot-iotkit/storages/KSQStorageFS.h>

using namespace VirgilIoTKit;

class KSQSecBox : public QObject, public VSQSingleton<KSQSecBox> {

    Q_OBJECT

    friend VSQSingleton<KSQSecBox>;

public:
    bool
    save(vs_secbox_type_t type, vs_storage_element_id_t id, const QByteArray &data);

    bool
    load(vs_storage_element_id_t id, QByteArray &data);

    bool
    del(vs_storage_element_id_t id);

    bool
    isValid() const {
        return m_valid;
    }

signals:

public slots:

private:
    KSQSecBox();
    virtual ~KSQSecBox();

    bool m_valid;

    KSQStorageFS *m_storage;
};

#endif // _YIOT_QT_SECURITY_BOX_H_
