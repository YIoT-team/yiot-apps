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

#include <virgil/iot/qt/VSQIoTKit.h>
#include <yiot-iotkit/netif/KSQUdp.h>

//-----------------------------------------------------------------------------
KSQUdp::KSQUdp(quint16 port) : m_port(port) {
    using std::placeholders::_1;
    using std::placeholders::_2;
    std::function<vs_status_e(const uint8_t *, const uint16_t)> _resend =
            std::bind(&KSQUdp::_internal_tx, this, _1, _2);
    m_resendContainer = new KSResendContainer(_resend);

    connect(&m_socket, &QUdpSocket::stateChanged, this, &VSQNetifBase::fireStateChanged);
    connect(&m_socket, &QUdpSocket::readyRead, this, &KSQUdp::onHasInputData);
}

//-----------------------------------------------------------------------------
bool
KSQUdp::init() {

    if (!m_socket.bind(m_port, QUdpSocket::ReuseAddressHint)) {
        VS_LOG_ERROR("Unable to bind LocalHost:%d. Last error : %s", m_port, VSQCString(m_socket.errorString()));
        return false;
    }

    for (auto &interface : QNetworkInterface::allInterfaces()) {
        if (interface.flags() & QNetworkInterface::IsLoopBack ||
            !(interface.flags() & QNetworkInterface::CanBroadcast && interface.flags() & QNetworkInterface::IsRunning &&
              interface.flags() & QNetworkInterface::IsUp)) {
            continue;
        }

        QString address = interface.hardwareAddress();
        if (address.isEmpty()) {
            continue;
        }

        m_mac = address;
        VS_LOG_INFO("Current MAC address: %s", VSQCString(m_mac.description()));
        break;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool
KSQUdp::deinit() {
    m_socket.close();
    return true;
}

//-----------------------------------------------------------------------------
vs_status_e
KSQUdp::_internal_tx(const uint8_t *data, const uint16_t data_sz) {
    auto sentBytes = m_socket.writeDatagram(
            QByteArray::fromRawData(reinterpret_cast<const char *>(data), data_sz), QHostAddress::Broadcast, m_port);

    if (sentBytes != data_sz) {
        VS_LOG_ERROR("Sent bytes : %d, data bytes to send : %d. Last error : %s",
                     sentBytes,
                     data_sz,
                     VSQCString(m_socket.errorString()));
        return VS_CODE_ERR_TX_SNAP;
    }

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
////--------------------------------------------
////  QoS 1
////--------------------------------------------
//if (_if_ack_packet(received_data, recv_sz)) {
//continue;
//}
//_send_ack(received_data, recv_sz);
////--------------------------------------------
////  ~QoS 1
////--------------------------------------------
bool
KSQUdp::tx(const QByteArray &data) {
    if (VS_CODE_OK == _internal_tx(reinterpret_cast<const uint8_t *>(data.data()), data.size())) {

        //--------------------------------------------
        //  QoS 1
        //--------------------------------------------
        vs_snap_packet_t *p = (vs_snap_packet_t *)data.data();
        if (0 != memcmp(p->eth_header.dest.bytes, vs_snap_broadcast_mac()->bytes, ETH_ADDR_LEN)) {
            m_resendContainer->addPacket(&p->eth_header.dest,
                                         p->header.transaction_id,
                                         reinterpret_cast<const uint8_t *>(data.data()),
                                         data.size());
        }
        //--------------------------------------------
        //  ~QoS 1
        //--------------------------------------------

        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
QString
KSQUdp::macAddr() const {

    return m_mac;
}

//-----------------------------------------------------------------------------
void
KSQUdp::onHasInputData() {

    while (m_socket.hasPendingDatagrams()) {
        processData(m_socket.receiveDatagram().data());
    }
}

//-----------------------------------------------------------------------------
void
KSQUdp::restart() {
    deinit();
    init();
}

//-----------------------------------------------------------------------------
