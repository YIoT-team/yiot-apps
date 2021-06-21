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

#include <controllers/extensions/device/KSQExtDevices.h>

//-----------------------------------------------------------------------------
KSQExtDevices::KSQExtDevices() {
    qRegisterMetaType<KSQExtDevice>("KSQExtDevice");

    loadBuiltinDevicesInfo();
}

//-----------------------------------------------------------------------------
QString
KSQExtDevices::readContent(const QString &fileName) {
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
KSQExtDevices::fixQrcQFile(const QString &resourceDir) {
    if (resourceDir.startsWith("qrc")) {
        return resourceDir.right(resourceDir.length() - 3);
    }
    return resourceDir;
}

//-----------------------------------------------------------------------------
bool
KSQExtDevices::loadOneBuiltinDevice(const QString &resourceDir) {
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
    const QString basePath = resourceDir + "/texts/" + lang;

    QString logo = resourceDir + "/logo/logo.png";
    QString name = readContent(basePath + "/title.txt");
    QString description = readContent(basePath + "/description.md");
    QString link = readContent(basePath + "/link.txt");

    if (!version.isEmpty() && !languages.isEmpty() && !logo.isEmpty() && !name.isEmpty() && !description.isEmpty() &&
        !link.isEmpty()) {
        m_devices << QSharedPointer<KSQExtDevice>::create(logo, name, version, description, link, size, languages);
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
bool
KSQExtDevices::loadBuiltinDevicesInfo() {
    QFile file;
    auto jsonFileName = fixQrcQFile("qrc:/qml/info/builtin-devices.json");
    file.setFileName(jsonFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        VS_LOG_ERROR("Cannot read a list of built-in extensions");
        return false;
    }

    auto jsonData = file.readAll();
    file.close();

    auto jsonDoc = QJsonDocument::fromJson(jsonData);
    auto jsonObject = jsonDoc.object();
    auto devicesArray = jsonObject.value(QString("devices")).toArray();

    for (const auto &val : devicesArray) {
        auto deviceInfoObject = val.toObject();
        auto resourcesDir = deviceInfoObject.value("dir").toString();
        if (loadOneBuiltinDevice(resourcesDir)) {
            m_builtIn << resourcesDir;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
QStringList
KSQExtDevices::builtInDevices() const {
    return m_builtIn;
}

//-----------------------------------------------------------------------------
int
KSQExtDevices::rowCount(const QModelIndex &parent) const {
    return m_devices.size();
}

//-----------------------------------------------------------------------------
int
KSQExtDevices::columnCount(const QModelIndex &parent) const {
    return 1;
}

//-----------------------------------------------------------------------------
QVariant
KSQExtDevices::data(const QModelIndex &index, int role) const {
    if (index.row() < m_devices.count()) {

        switch (role) {
        case Element::Info:
            QVariant res;
            res.setValue(m_devices.at(index.row()).get());
            return res;
        }
    }

    return QVariant();
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray>
KSQExtDevices::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Info] = "info";
    return roles;
}

//-----------------------------------------------------------------------------
