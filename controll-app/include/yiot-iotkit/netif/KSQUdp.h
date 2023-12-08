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

#ifndef YIOT_IOTKIT_QT_UDP_H_
#define YIOT_IOTKIT_QT_UDP_H_

#include <QtCore>
#include <QtNetwork>

#include <virgil/iot/qt/protocols/snap/VSQNetifBase.h>
#include "qos1/resend-container.h"

class KSQUdp final : public VSQNetifBase {
    Q_OBJECT
public:
    KSQUdp(QHostAddress subnet, quint16 port = 4100);

    KSQUdp(KSQUdp const &) = delete;

    KSQUdp &
    operator=(KSQUdp const &) = delete;

    virtual ~KSQUdp() = default;

    virtual QAbstractSocket::SocketState
    connectionState() const override {
        return m_socket.state();
    }

    virtual bool
    requiresAdditionalActivation() const override {
        return false;
    }

    void
    restart();

    void
    setSubnet(QHostAddress subnet);

protected:
    bool
    init() override;

    bool
    deinit() override;

    bool
    tx(const QByteArray &data) override;

    QString
    macAddr() const override;

private slots:
    void
    onHasInputData();

    bool
    sendAck(const QByteArray &data);

    bool
    ifAckPacket(const QByteArray &data);

private:
    QHostAddress m_subnet;
    quint16 m_port;
    QUdpSocket m_socket;
    VSQMac m_mac;
    KSResendContainer *m_resendContainer;

public:
    vs_status_e
    _internal_tx(const uint8_t *data, const uint16_t data_sz);
};

#endif // YIOT_IOTKIT_QT_UDP_H_
