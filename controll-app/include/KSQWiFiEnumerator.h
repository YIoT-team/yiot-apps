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

#ifndef PROVISION_QT_WIFI_ENUM_H
#define PROVISION_QT_WIFI_ENUM_H

#include <QtCore>
#include <QAbstractTableModel>
#include <QNetworkInformation>

#include <virgil/iot/qt/VSQIoTKit.h>

#include <KSWiFi.h>

class KSQWiFiEnumerator : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Element { Name = Qt::UserRole, RSSI, ElementMax };

    KSQWiFiEnumerator();
    virtual ~KSQWiFiEnumerator();

    /**
     * QAbstractTableModel implementation
     */
    int
    rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int
    columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant
    data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray>
    roleNames() const override;

public slots:

signals:

private slots:
    void
    start();

    void
    stop();

    void
    onFindWiFi();

    void
    updateList(KSQWiFiNetworks &list);

private:
    KSQWiFiNetworks m_wifiList;
    QNetworkInformation *m_ni = QNetworkInformation::instance();

#if defined(Q_OS_ANDROID)
    KSQWiFiNetworks
    wifi_enum();
#endif

#if defined(Q_OS_MACOS) || defined(Q_OS_ANDROID)
    static const int kScanPeriodMs = 5000;
    QTimer m_timer;
#else
    QStringList
    _findWiFiGeneral();
#endif
};

#endif // PROVISION_QT_WIFI_ENUM_H
