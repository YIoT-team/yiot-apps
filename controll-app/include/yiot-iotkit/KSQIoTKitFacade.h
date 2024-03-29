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

#ifndef YIOT_IOTKIT_QT_FACADE_H
#define YIOT_IOTKIT_QT_FACADE_H

#include <QtCore>
#include <QtNetwork>
#include <virgil/iot/logger/logger.h>
#include <virgil/iot/provision/provision-structs.h>
#include <virgil/iot/qt/helpers/VSQFeatures.h>
#include <virgil/iot/qt/helpers/VSQImplementations.h>
#include <virgil/iot/qt/helpers/VSQAppConfig.h>
#include <virgil/iot/qt/helpers/VSQSingleton.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapServiceBase.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapINFOClient.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapCFGClient.h>
#include <virgil/iot/qt/protocols/snap/VSQSnapSnifferQml.h>

//   YIoT Extension for IoTKit
#include <yiot-iotkit/KSQFeatures.h>

class KSQIoTKitFacade : public QObject, public VSQSingleton<KSQIoTKitFacade> {
    Q_OBJECT

public:
    bool
    init(const KSQFeatures &features, const VSQImplementations &impl, const VSQAppConfig &appConfig);

    using VSQSnapSnifferPtr = QSharedPointer<VSQSnapSnifferQml>;

    VSQSnapSnifferPtr
    snapSniffer() {
        return m_snapSniffer;
    }
    VSQSnapCfgClient &
    snapCfgClient() {
        return VSQSnapCfgClient::instance();
    }

    virtual ~KSQIoTKitFacade();

    void
    updateAll();

    void
    requestInfoPC();

private slots:

    void
    onNetifProcess(struct VirgilIoTKit::vs_netif_t *netif, QByteArray data);

private:
    KSQFeatures m_features;
    VSQImplementations m_impl;
    VSQAppConfig m_appConfig;
    VSQSnapSnifferPtr m_snapSniffer;
    QThread *m_snapProcessorThread;

    void
    initSnap();

    void
    registerService(VSQSnapServiceBase &service);

    static VirgilIoTKit::vs_status_e
    netifProcessCb(struct VirgilIoTKit::vs_netif_t *netif, const uint8_t *data, const uint16_t data_sz);

    static bool
    needEncCb(vs_snap_service_id_t service_id, vs_snap_element_t element_id);
};

#endif // YIOT_IOTKIT_QT_FACADE_H
