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
KSQIntegrationsController::load(QSharedPointer<KSQOneExtension> extension) {
    QString integrationDir(extension->basePath());
    if (!m_engine) {
        VS_LOG_WARNING("QML Engine is not set");
        return false;
    }
    // Create JS processor
    const QString js = integrationDir + "/src/js/main.qml";
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
    const QString controlPage = integrationDir + "/src/qml/Control.qml";
    QVariant res;
    if (QMetaObject::invokeMethod(deviceControlContainer,
                                  "addIntegrationControl",
                                  Q_RETURN_ARG(QVariant, res),
                                  Q_ARG(QVariant, QVariant::fromValue(controlPage)),
                                  Q_ARG(QVariant, QVariant::fromValue(extension.get())))) {

        // Let know to JS about view controller
        object->setProperty("controlPageIdx", res);

        // Set JS processor for extension
        extension->setJs(object);

        // Connect QML signals
        QObject::connect(object, SIGNAL(activated(QString, QString)), this, SIGNAL(fireActivate(QString, QString)));

        QObject::connect(object, SIGNAL(deactivated(QString)), this, SIGNAL(fireDeactivate(QString)));

        // Inform JS code about finish of integration load
        QMetaObject::invokeMethod(object, "onLoad", Qt::QueuedConnection);

    } else {
        VS_LOG_ERROR("Cannot register device control page");
    }

    return true;
}

//-----------------------------------------------------------------------------
