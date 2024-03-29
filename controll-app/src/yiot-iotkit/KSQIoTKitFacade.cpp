//  Copyright (C) 2015-2020 Virgil Security, Inc.
//
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//      (1) Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//      (2) Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in
//      the documentation and/or other materials provided with the
//      distribution.
//
//      (3) Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
//  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
//  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
//  Lead Maintainer: Virgil Security Inc. <support@virgilsecurity.com>

#include <virgil/iot/qt/protocols/snap/VSQNetifBase.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapServiceBase.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapINFOClient.h>
#include <virgil/iot/qt/VSQIoTKit.h>

#include <yiot-iotkit/KSQIoTKitFacade.h>
#include <yiot-iotkit/snap/KSQSnapUSERClient.h>
#include <yiot-iotkit/snap/KSQSnapPRVSClient.h>
#include <yiot-iotkit/snap/KSQSnapSCRTClient.h>
#include <yiot-iotkit/provision/KSQProvision.h>
#include <yiot-iotkit/root-of-trust/KSQRoTController.h>

#include <virgil/iot/session/session.h>

using namespace VirgilIoTKit;

//-----------------------------------------------------------------------------
KSQIoTKitFacade::~KSQIoTKitFacade() {
    m_snapProcessorThread->terminate();
    m_snapProcessorThread->wait();
    delete m_snapProcessorThread;
}

//-----------------------------------------------------------------------------
bool
KSQIoTKitFacade::init(const KSQFeatures &features, const VSQImplementations &impl, const VSQAppConfig &appConfig) {

    // Register types
    qRegisterMetaType<VirgilIoTKit::vs_netif_t *>("VirgilIoTKit::vs_netif_t*");
    qRegisterMetaType<VSQDeviceInfo>("VSQDeviceInfo");
    qRegisterMetaType<QAbstractSocket::SocketState>();
    qRegisterMetaType<vs_mac_addr_t>("vs_mac_addr_t");
    qRegisterMetaType<VSQMac>("VSQMac");

    // Process events in separate thread
    m_snapProcessorThread = new QThread();
    m_snapProcessorThread->start();
    moveToThread(m_snapProcessorThread);

    // Prepare provision
    auto &provision = KSQProvision::instance();
    auto &rotContloller = KSQRoTController::instance();
    if (!provision.isValid()) {
        rotContloller.drop();
        if (!provision.create(rotContloller.localRootOfTrust())) {
            VS_LOG_WARNING("Cannot create provision");
            return false;
        }
    }

    m_features = features;
    m_impl = impl;
    m_appConfig = appConfig;

    vs_logger_init(appConfig.logLevel());

    try {
        if (features.hasSnap()) {
            initSnap();
        }

        return true;

    } catch (QString &descr) {
        VS_LOG_CRITICAL("Error during Virgil IoT KIT initialization : %s", descr.toStdString().c_str());
        return false;
    }
}

//-----------------------------------------------------------------------------
bool
KSQIoTKitFacade::needEncCb(vs_snap_service_id_t service_id, vs_snap_element_t element_id) {
    if (VS_PC_SERVICE_ID == service_id && VS_PC_PCMD == element_id) {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
void
KSQIoTKitFacade::initSnap() {

    if (!m_impl.netifs().count() || !m_impl.netifs().first()) {
        throw QString("There is no default network implementation");
    }

    if (vs_snap_init(m_impl.netifs().first()->lowLevelNetif(),
                     netifProcessCb,
                     needEncCb,
                     NULL,
                     m_appConfig.manufactureId(),
                     m_appConfig.deviceType(),
                     m_appConfig.deviceSerial(),
                     m_appConfig.deviceRoles()) != VirgilIoTKit::VS_CODE_OK) {
        throw QString("Unable to initialize SNAP");
    }

    // Add additional network interfaces
    bool isDefault = true;
    foreach (auto &netif, m_impl.netifs()) {
        if (isDefault) {
            isDefault = false;
            continue;
        }

        if (VirgilIoTKit::VS_CODE_OK != vs_snap_netif_add(netif->lowLevelNetif())) {
            throw QString("Unable to add SNAP network interface");
        }
    }

    // Setup Session
    vs_mac_addr_t default_mac;
    vs_snap_mac_addr(0, &default_mac);
    vs_session_init(KSQSecModule::instance().secmoduleImpl(), default_mac.bytes);

    if (m_features.hasFeature(KSQFeatures::SNAP_PRVS_CLIENT)) {
        registerService(KSQSnapPRVSClient::instance());
    }

    if (m_features.hasFeature(KSQFeatures::SNAP_SCRT_CLIENT)) {
        registerService(KSQSnapSCRTClient::instance());
    }

    if (m_features.hasFeature(KSQFeatures::SNAP_INFO_CLIENT)) {
        registerService(VSQSnapInfoClient::instance());
    }

    if (m_features.hasFeature(KSQFeatures::SNAP_SNIFFER)) {
        m_snapSniffer = decltype(m_snapSniffer)::create(m_appConfig.snifferConfig(), m_impl.netifs().first());
    }

    if (m_features.hasFeature(KSQFeatures::SNAP_CFG_CLIENT)) {
        registerService(VSQSnapCfgClient::instance());
    }

    if (m_features.hasFeature(KSQFeatures::SNAP_PC_CLIENT)) {
        registerService(KSQSnapUSERClient::instance());
    }
}

//-----------------------------------------------------------------------------
void
KSQIoTKitFacade::updateAll() {
    requestInfoPC();

    if (m_features.hasFeature(KSQFeatures::SNAP_INFO_CLIENT)) {
        VSQSnapInfoClient::instance().onStartFullPolling();
    }
}

//-----------------------------------------------------------------------------
void
KSQIoTKitFacade::requestInfoPC() {
    if (m_features.hasFeature(KSQFeatures::SNAP_PC_CLIENT)) {
        KSQSnapUSERClient::instance().requestState(broadcastMac);
    }
}

//-----------------------------------------------------------------------------
void
KSQIoTKitFacade::registerService(VSQSnapServiceBase &service) {
    if (vs_snap_register_service(service.serviceInterface()) != VirgilIoTKit::VS_CODE_OK) {
        throw QString("Unable to register SNAP's %1 service").arg(service.serviceName());
    }
}

//-----------------------------------------------------------------------------
void
KSQIoTKitFacade::onNetifProcess(struct VirgilIoTKit::vs_netif_t *netif, QByteArray data) {
    vs_snap_default_processor(netif, reinterpret_cast<const uint8_t *>(data.data()), data.length());
}

//-----------------------------------------------------------------------------
vs_status_e
KSQIoTKitFacade::netifProcessCb(struct vs_netif_t *netif, const uint8_t *data, const uint16_t data_sz) {
    vs_snap_packet_dump("IN ", (vs_snap_packet_t *)data);

    auto ba = QByteArray(reinterpret_cast<const char *>(data), data_sz);
    if (!QMetaObject::invokeMethod(&instance(),
                                   "onNetifProcess",
                                   Qt::QueuedConnection,
                                   Q_ARG(VirgilIoTKit::vs_netif_t *, netif),
                                   Q_ARG(QByteArray, ba))) {
        VS_LOG_ERROR("PACKET CANNOT BE PROCESSED !!!");
    }
    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
