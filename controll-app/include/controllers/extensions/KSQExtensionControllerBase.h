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

#ifndef YIOT_EXTENSION_CONTROLLER_BASE_H
#define YIOT_EXTENSION_CONTROLLER_BASE_H

#include <QQmlApplicationEngine>
#include <QAbstractTableModel>

#include <common/protocols/snap/user/user-structs.h>
#include <controllers/extensions/KSQOneExtension.h>

class KSQExtensionControllerBase {
public:
    KSQExtensionControllerBase() = default;
    virtual ~KSQExtensionControllerBase() = default;

    virtual bool
    load(QSharedPointer<KSQOneExtension> extension) = 0;

    QSharedPointer<QObject>
    qmlProcessor(size_t pos) {
        if (m_qmlProcessor.keys().contains(pos)) {
            return m_qmlProcessor[pos];
        }
        return QSharedPointer<QObject>();
    }

    void
    setQmlEngine(QQmlApplicationEngine *engine) {
        m_engine = engine;
    }

protected:
    QMap<size_t, QSharedPointer<QObject>> m_qmlProcessor;

    QQmlApplicationEngine *m_engine;
};

#endif // YIOT_EXTENSION_CONTROLLER_BASE_H
