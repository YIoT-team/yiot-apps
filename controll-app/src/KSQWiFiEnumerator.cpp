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
#include "os/android/KSQAndroid.h"
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
    m_ni->loadDefaultBackend();
    m_ni = QNetworkInformation::instance();
    connect(m_ni, &QNetworkInformation::reachabilityChanged, this, &KSQWiFiEnumerator::onFindWiFi);
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
#if defined(Q_OS_ANDROID)
KSQWiFiNetworks
KSQWiFiEnumerator::wifi_enum() {
    return KSQAndroid::enumWifi();
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
