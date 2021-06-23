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

#include <QSharedPointer>

#include "virgil/iot/logger/logger.h"

#include <controllers/extensions/KSQExtensions.h>

//-----------------------------------------------------------------------------
KSQExtensions::KSQExtensions(const QString &prefix, QSharedPointer<KSQExtensionControllerBase> processor) {

    m_prefix = prefix;
    m_processor = processor;
}

//-----------------------------------------------------------------------------
bool
KSQExtensions::load() {
    bool res;
    beginResetModel();
    m_extensions.clear();
    res = loadBuiltinExtensions();
    endResetModel();
    return res;
}
//-----------------------------------------------------------------------------
QString
KSQExtensions::readContent(const QString &fileName) {
    QFile file;
    file.setFileName(fixQrcQFile(fileName));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        VS_LOG_ERROR("Cannot read file %s", fileName.toStdString().c_str());
        return "";
    }

    auto text = file.readAll();
    file.close();

    return text;
}

//-----------------------------------------------------------------------------
QString
KSQExtensions::fixQrcQFile(const QString &resourceDir) {
    if (resourceDir.startsWith("qrc")) {
        return resourceDir.right(resourceDir.length() - 3);
    }
    return resourceDir;
}

//-----------------------------------------------------------------------------
bool
KSQExtensions::loadOneExtension(const QString &resourceDir) {
    QFile file;
    auto jsonFileName = fixQrcQFile(resourceDir + "/info.json");
    file.setFileName(jsonFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        VS_LOG_ERROR("Cannot read a built-in device information %s", resourceDir.toStdString().c_str());
        return false;
    }

    auto jsonData = file.readAll();
    file.close();

    auto jsonDoc = QJsonDocument::fromJson(jsonData);
    auto jsonObject = jsonDoc.object();

    QString version = jsonObject.value("version").toString();
    size_t size = jsonObject.value("size").toInt();
    QStringList languages;
    foreach (QVariant v, jsonObject.value("languages").toArray().toVariantList()) { languages << v.value<QString>(); }

    const QString lang = "en";
    const QString basePath = resourceDir + "/info/texts/" + lang;

    QString logo = resourceDir + "/info/logo/logo.png";
    QString name = readContent(basePath + "/title.txt");
    QString description = readContent(basePath + "/description.md");
    QString link = readContent(basePath + "/link.txt");

    if (!version.isEmpty() && !languages.isEmpty() && !logo.isEmpty() && !name.isEmpty() && !description.isEmpty() &&
        !link.isEmpty()) {
        m_extensions << QSharedPointer<KSQOneExtension>::create(
                logo, name, version, description, link, size, languages);
        if (!m_processor.isNull()) {
            m_processor->load(resourceDir, m_extensions.last());
        }
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
bool
KSQExtensions::loadBuiltinExtensions() {
    QFile file;
    auto jsonFileName = fixQrcQFile(QString("qrc:/qml/info/builtin-%1.json").arg(m_prefix));
    file.setFileName(jsonFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        VS_LOG_ERROR("Cannot read a list of built-in extensions");
        return false;
    }

    auto jsonData = file.readAll();
    file.close();

    auto jsonDoc = QJsonDocument::fromJson(jsonData);
    auto jsonObject = jsonDoc.object();
    auto devicesArray = jsonObject.value(QString(m_prefix)).toArray();

    for (const auto &val : devicesArray) {
        auto deviceInfoObject = val.toObject();
        auto resourcesDir = deviceInfoObject.value("dir").toString();
        if (loadOneExtension(resourcesDir)) {
            m_builtIn << resourcesDir;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
QStringList
KSQExtensions::builtInExtensions() const {
    return m_builtIn;
}

//-----------------------------------------------------------------------------
int
KSQExtensions::rowCount(const QModelIndex &parent) const {
    return m_extensions.size();
}

//-----------------------------------------------------------------------------
int
KSQExtensions::columnCount(const QModelIndex &parent) const {
    return 1;
}

//-----------------------------------------------------------------------------
QVariant
KSQExtensions::data(const QModelIndex &index, int role) const {
    if (index.row() < m_extensions.count()) {

        switch (role) {
        case Element::Info: {
            QVariant res;
            res.setValue(m_extensions.at(index.row()).get());
            return res;
        }

        case Element::Js: {
            QVariant res;
            res.setValue(m_processor->qmlProcessor().get());
            return res;
        }
        }
    }

    return QVariant();
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray>
KSQExtensions::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Info] = "info";
    roles[Js] = "js";
    return roles;
}

//-----------------------------------------------------------------------------
