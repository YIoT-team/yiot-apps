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

#include <KSQWiFiEnumerator.h>

#include <thread>

#ifdef Q_OS_ANDROID
#include "android/KSQAndroid.h"
#endif // Q_OS_ANDROID

#if 1
// TODO: Remove after fixing of deprecated functionality
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#if !defined(WIFI_ENUM_DEBUG)
#define WIFI_ENUM_DEBUG 0
#endif

//-----------------------------------------------------------------------------
KSQWiFiEnumerator::KSQWiFiEnumerator() {
#if defined(Q_OS_MACOS) || defined(Q_OS_ANDROID)
    m_timer.setSingleShot(false);
    m_timer.setInterval(kScanPeriodMs);
    connect(&m_timer, &QTimer::timeout, this, &KSQWiFiEnumerator::onFindWiFi);
#else
    connect(&m_ncm, &QNetworkConfigurationManager::updateCompleted, this, &KSQWiFiEnumerator::onFindWiFi);
#endif

    QTimer::singleShot(200, [this]() { start(); });
}

//-----------------------------------------------------------------------------
KSQWiFiEnumerator::~KSQWiFiEnumerator() {
    stop();
}

//-----------------------------------------------------------------------------
void
KSQWiFiEnumerator::start() {
#if defined(Q_OS_MACOS) || defined(Q_OS_ANDROID)
    m_timer.start();
#else
    m_ncm.updateConfigurations();
#endif
    onFindWiFi();
}

//-----------------------------------------------------------------------------
void
KSQWiFiEnumerator::stop() {
#if defined(Q_OS_MACOS)
    m_timer.stop();
#endif
}

//-----------------------------------------------------------------------------
QStringList wifiListHandler(QString &input_list, QRegExp &rx) {
    QStringList list;
    int pos = 0;
    while ((pos = rx.indexIn(input_list, pos)) != -1) {
                list << rx.cap(1);
                pos += rx.matchedLength();
    }

    return list;
}
//-----------------------------------------------------------------------------
#if !defined(Q_OS_MACOS) && !defined(Q_OS_WIN32)
KSQWiFiNetworks
KSQWiFiEnumerator::wifi_enum() {
#ifdef Q_OS_ANDROID
    return KSQAndroid::enumWifi();
#else
    QStringList ssid_list;
    QStringList rssi_list;
    QProcess wifiProcess;

    wifiProcess.start("sh", QStringList() << "-c" << "nmcli -f SSID dev wifi");
    if (wifiProcess.waitForFinished()) {
        QString raw_list = QTextCodec::codecForMib(106)->toUnicode(wifiProcess.readAll());
        QRegExp ssid_rx("[\\S\\s]([a-zA-Z0-9_-]+)[\\S\\s]");
        QStringList out_list = wifiListHandler(raw_list, ssid_rx);
        out_list.removeFirst();
        ssid_list = out_list;
        wifiProcess.close();
    }

    qDebug() << "++++++++++++++++++[FILTRED_SSID_LIST]++++++++++++++++++: " << ssid_list;

    wifiProcess.start("sh", QStringList() << "-c" << "nmcli -f SIGNAL dev wifi");
    if (wifiProcess.waitForFinished()) {
        QString raw_list = QTextCodec::codecForMib(106)->toUnicode(wifiProcess.readAll());
        QRegExp rssi_rx("(\\d+)");
        QStringList out_list = wifiListHandler(raw_list, rssi_rx);
        rssi_list = out_list;
        wifiProcess.close();
    }

    qDebug() << "++++++++++++++++++[FILTRED_RSSI_LIST]++++++++++++++++++: " << rssi_list;

    KSQWiFiNetworks wifiList;
    for (int i = 0; i < ssid_list.size(); i++) {
        int current_rssi = rssi_list[i].toInt();
        wifiList[ssid_list[i]] = current_rssi;
    }
    return wifiList;
#endif
}
#endif

//-----------------------------------------------------------------------------
void
KSQWiFiEnumerator::onFindWiFi() {
    std::thread t([this]() {
        auto list = wifi_enum();
        QMetaObject::invokeMethod(this, "updateList", Qt::BlockingQueuedConnection, Q_ARG(KSQWiFiNetworks &, list));
    });

    t.detach();
}

//-----------------------------------------------------------------------------
void
KSQWiFiEnumerator::updateList(KSQWiFiNetworks &list) {
    const bool isSame = list.keys() == m_wifiList.keys();
    m_wifiList = list;

    QVector<int> roles;
    if (isSame) {
        roles << RSSI;
    } else {
        beginResetModel();
        endResetModel();
    }

    auto topLeft = createIndex(0, 0);
    auto bottomRight = createIndex(m_wifiList.count(), 0);
    emit dataChanged(topLeft, bottomRight, roles);

#if WIFI_ENUM_DEBUG
    qDebug() << m_wifiList.keys();
#endif
}

//-----------------------------------------------------------------------------
int
KSQWiFiEnumerator::rowCount(const QModelIndex &parent) const {
    return m_wifiList.count();
}

//-----------------------------------------------------------------------------
int
KSQWiFiEnumerator::columnCount(const QModelIndex &paren) const {
    return 1;
}

//-----------------------------------------------------------------------------
QVariant
KSQWiFiEnumerator::data(const QModelIndex &index, int role) const {
    if (index.row() < m_wifiList.count()) {
        auto key = m_wifiList.keys().at(index.row());

        switch (role) {
        case Element::Name:
            return key;

        case Element::RSSI:
            return m_wifiList[key].rssi;
        }
    }

    return QVariant();
}

//-----------------------------------------------------------------------------
QHash<int, QByteArray>
KSQWiFiEnumerator::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[RSSI] = "rssi";
    return roles;
}

//-----------------------------------------------------------------------------

#if 1
#pragma GCC diagnostic pop
#endif
