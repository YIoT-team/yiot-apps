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

#include <controllers/integrations/KSQIntegrationsController.h>

//-----------------------------------------------------------------------------
bool
KSQIntegrationsController::load(const QString &integrationDir, QSharedPointer<KSQOneExtension> extension) {
    if (!m_engine) {
        VS_LOG_WARNING("QML Engine is not set");
        return false;
    }
    // Create JS processor
    const QString js = integrationDir + "/js/main.qml";
    QQmlComponent component(m_engine, QUrl(js));
    if (component.isError()) {
        qDebug() << component.errors();
    }
    QObject *object = component.create();
    if (!object) {
        VS_LOG_ERROR("Cannot create QML processor for device type : %s", integrationDir.toStdString().c_str());
        return false;
    }
    m_qmlProcessor.reset(object);

    // Add Device control page
    auto *rootObj = m_engine->rootObjects().first();
    auto deviceControlContainer = rootObj->findChild<QObject *>("integrationControlContainer");
    const QString controlPage = integrationDir + "/qml/Control.qml";
    QVariant res;
    if (QMetaObject::invokeMethod(deviceControlContainer,
                                  "addIntegrationControl",
                                  Q_RETURN_ARG(QVariant, res),
                                  Q_ARG(QVariant, QVariant::fromValue(controlPage)),
                                  Q_ARG(QVariant, QVariant::fromValue(extension.get())))) {
        object->setProperty("controlPageIdx", res);
        extension->setJs(object);

        QMetaObject::invokeMethod(object, "onLoad", Qt::QueuedConnection);

    } else {
        VS_LOG_ERROR("Cannot register device control page");
    }

    return true;
}

//-----------------------------------------------------------------------------
