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

#include <QJsonObject>

#include <virgil/iot/qt/VSQIoTKit.h>
#include <yiot-iotkit/netif/KSQNetifWebsocket.h>

const QString KSQNetifWebsocket::_accountIdTag("account_id");
const QString KSQNetifWebsocket::_payloadTag("payload");

//-----------------------------------------------------------------------------
KSQNetifWebsocket::KSQNetifWebsocket() : m_canCommunicate(false) {
    m_objConnections << connect(&m_webSocket, &QWebSocket::connected, this, &KSQNetifWebsocket::onConnected);
    m_objConnections << connect(&m_webSocket, &QWebSocket::disconnected, this, &KSQNetifWebsocket::onDisconnected);
    m_objConnections << connect(&m_webSocket, &QWebSocket::stateChanged, this, &KSQNetifWebsocket::onStateChanged);
    m_objConnections << connect(
            &m_webSocket, &QWebSocket::textMessageReceived, this, &KSQNetifWebsocket::onMessageReceived);
    m_objConnections << connect(&m_webSocket,
                                SIGNAL(error(QAbstractSocket::SocketError)),
                                this,
                                SLOT(onError(QAbstractSocket::SocketError)));

    m_objConnections << connect(
            this, SIGNAL(fireSend(QByteArray)), this, SLOT(onSend(QByteArray)), Qt::QueuedConnection);
}

//-----------------------------------------------------------------------------
KSQNetifWebsocket::~KSQNetifWebsocket() {
    for (auto &conn : m_objConnections) {
        disconnect(conn);
    }
    disconnectWS();
}

//-----------------------------------------------------------------------------
bool
KSQNetifWebsocket::init() {
    // TODO: Fix it
    m_mac = VSQMac("01:02:03:04:05:06");
    if (!m_url.isEmpty()) {
        m_canCommunicate = true;
        m_webSocket.open(m_url);
    }
    return true;
}

//-----------------------------------------------------------------------------
bool
KSQNetifWebsocket::deinit() {
    m_canCommunicate = false;
    return true;
}

//-----------------------------------------------------------------------------
void
KSQNetifWebsocket::onSend(QByteArray data) {
    if (!isActive()) {
        return;
    }

    QJsonObject json;
    json[_accountIdTag] = m_account;
    json[_payloadTag] = QString(data.toBase64());

    m_webSocket.sendBinaryMessage(QJsonDocument(json).toJson());
}

//-----------------------------------------------------------------------------
bool
KSQNetifWebsocket::tx(const QByteArray &data) {
    emit fireSend(data);
    return true;
}

//-----------------------------------------------------------------------------
QString
KSQNetifWebsocket::macAddr() const {
    return m_mac;
}

//-----------------------------------------------------------------------------
void
KSQNetifWebsocket::registerReceiver() {
    tx(QByteArray());
}

//-----------------------------------------------------------------------------
void
KSQNetifWebsocket::onStateChanged(QAbstractSocket::SocketState state) {
    qDebug() << "KSQNetifWebsocket::onStateChanged : " << state;
}

//-----------------------------------------------------------------------------
void
KSQNetifWebsocket::onError(QAbstractSocket::SocketError error) {
    qDebug() << "KSQNetifWebsocket::onError : " << error;
    QTimer::singleShot(0, [this]() {
        m_webSocket.open(m_url);
        emit fireStateChanged(m_webSocket.state());
    });
}

//-----------------------------------------------------------------------------
void
KSQNetifWebsocket::onConnected() {
    VS_LOG_DEBUG("WebSocket connected");
    registerReceiver();

    emit fireStateChanged(m_webSocket.state());

    if (isActive()) {
        emit fireDeviceReady();
    }
}

//-----------------------------------------------------------------------------
void
KSQNetifWebsocket::onDisconnected() {
    VS_LOG_DEBUG("WebSocket disconnected");
}

//-----------------------------------------------------------------------------
void
KSQNetifWebsocket::onMessageReceived(QString message) {

    if (isActive()) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QByteArray dataBase64 = QByteArray::fromStdString((jsonObject[_payloadTag].toString().toStdString()));
        QByteArray data = QByteArray::fromBase64(dataBase64);
        processData(data);
    }
}

//-----------------------------------------------------------------------------
bool
KSQNetifWebsocket::isActive() const {
    return QAbstractSocket::ConnectedState == m_webSocket.state() && m_canCommunicate;
}

//-----------------------------------------------------------------------------
QAbstractSocket::SocketState
KSQNetifWebsocket::connectionState() const {
    return m_webSocket.state();
}

//-----------------------------------------------------------------------------
bool
KSQNetifWebsocket::connectWS(const QString &url) {
    if ((url.startsWith("ws:/") || url.startsWith("wss:/")) && url.contains("&")) {
        auto parts = url.split("&", Qt::SkipEmptyParts);
        if (parts.length() == 2) {
            m_url = QUrl(parts.first());
            m_account = parts.last();

            m_canCommunicate = true;
            m_webSocket.open(m_url);

            return true;
        }
    }

    disconnectWS();
    return false;
}

//-----------------------------------------------------------------------------
void
KSQNetifWebsocket::disconnectWS() {
    m_canCommunicate = false;
    m_webSocket.close();
}

//-----------------------------------------------------------------------------
