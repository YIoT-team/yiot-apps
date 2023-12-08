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

#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>

#include <fstream>
#include <streambuf>

#include <atomic>

#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/protocols/snap/snap-structs.h>
#include <virgil/iot/logger/logger.h>

#include "qos1/resend-container.h"

static vs_status_e
_udp_init(struct vs_netif_t *netif, const vs_netif_rx_cb_t rx_cb, const vs_netif_process_cb_t process_cb);

static vs_status_e
_udp_deinit(struct vs_netif_t *netif);

static vs_status_e
_udp_tx(struct vs_netif_t *netif, const uint8_t *data, const uint16_t data_sz);

static vs_status_e
_udp_mac(const struct vs_netif_t *netif, struct vs_mac_addr_t *mac_addr);

static vs_status_e
_internal_udp_tx(const uint8_t *data, const uint16_t data_sz);

static vs_netif_t _netif_udp_ = {.user_data = NULL,
        .init = _udp_init,
        .deinit = _udp_deinit,
        .tx = _udp_tx,
        .mac_addr = _udp_mac,
        .packet_buf_filled = 0};

static vs_netif_rx_cb_t _netif_udp_rx_cb = 0;
static vs_netif_process_cb_t _netif_udp_process_cb = 0;

static int _udp_sock = -1;
static pthread_t receive_thread;
static pthread_t connect_thread;
static uint8_t _mac_addr[6] = {2, 2, 2, 2, 2, 2};

static in_addr_t _dst_addr = INADDR_BROADCAST;

static KSResendContainer *_resendContainer = nullptr;
static std::atomic<bool> _ready = false;
static std::atomic<bool> _connecting = false;

#if __APPLE__
static std::string _defaultNetif = "en0";
#else
static std::string _defaultNetif = "eth0";
#endif

#define UDP_BCAST_PORT (4100)

#define RX_BUF_SZ (2048)

//-----------------------------------------------------------------------------
static bool
_if_ack_packet(const uint8_t *data, size_t data_sz) {
    // It's a packet with empty content
    if (data_sz != sizeof(vs_snap_packet_t)) {
        return false;
    }

    vs_snap_packet_t *p = (vs_snap_packet_t *)data;

    // Element ID and Service ID should be zero
    if (p->header.element_id || p->header.service_id) {
        return false;
    }

    // Destination MAC is my MAC address
    if (0 != memcmp(p->eth_header.dest.bytes, _mac_addr, ETH_ADDR_LEN)) {
        return false;
    }

    // Process ACK
    _resendContainer->processResponse(&p->eth_header.src, p->header.transaction_id);

    return true;
}

//-----------------------------------------------------------------------------
static bool
_send_ack(const uint8_t *data, size_t data_sz) {
    // Packet should be correct
    if (data_sz < sizeof(vs_snap_packet_t)) {
        return false;
    }

    vs_snap_packet_t *p = (vs_snap_packet_t *)data;

    // Destination MAC is my MAC address
    if (0 != memcmp(p->eth_header.dest.bytes, _mac_addr, ETH_ADDR_LEN)) {
        return false;
    }

    // Send ACK
    vs_snap_packet_t ack;
    memset(&ack, 0, sizeof(ack));
    memcpy(ack.eth_header.src.bytes, p->eth_header.dest.bytes, ETH_ADDR_LEN);
    memcpy(ack.eth_header.dest.bytes, p->eth_header.src.bytes, ETH_ADDR_LEN);
    ack.header.transaction_id = p->header.transaction_id;
    _internal_udp_tx((const uint8_t *)&ack, sizeof(ack));

    return true;
}

//-----------------------------------------------------------------------------
static void *
_udp_receive_processor(void *sock_desc) {
    static uint8_t received_data[RX_BUF_SZ];
    struct sockaddr_in client_addr;
    ssize_t recv_sz;
    socklen_t addr_sz = sizeof(struct sockaddr_in);
    const uint8_t *packet_data = NULL;
    uint16_t packet_data_sz = 0;

    vs_log_thread_descriptor("udp bcast thr");

    while (1) {
        // Wait for a connection
        if (!_ready) {
            usleep(300 * 1000);
            continue;
        }

        memset(received_data, 0, RX_BUF_SZ);

        recv_sz =
                recvfrom(_udp_sock, received_data, sizeof received_data, 0, (struct sockaddr *)&client_addr, &addr_sz);
        if (recv_sz < 0) {
            VS_LOG_ERROR("UDP broadcast: recv stop");
            break;
        }

        if (!recv_sz) {
            continue;
        }

        //--------------------------------------------
        //  QoS 1
        //--------------------------------------------
        if (_if_ack_packet(received_data, recv_sz)) {
            continue;
        }
        _send_ack(received_data, recv_sz);
        //--------------------------------------------
        //  ~QoS 1
        //--------------------------------------------

        // Pass received data to upper level via callback
        if (_netif_udp_rx_cb) {
            if (0 == _netif_udp_rx_cb(&_netif_udp_, received_data, recv_sz, &packet_data, &packet_data_sz)) {
                // Ready to process packet
                if (_netif_udp_process_cb) {
                    vs_snap_packet_dump("IN ", (vs_snap_packet_t *)packet_data);
                    _netif_udp_process_cb(&_netif_udp_, packet_data, packet_data_sz);
                }
            }
        }
    }

    return NULL;
}

//-----------------------------------------------------------------------------
static in_addr_t
_get_interface_bcast_addr(const char *name) {
    int family;
    struct ifreq ifreq;
    char host[128];
    memset(&ifreq, 0, sizeof ifreq);
    strncpy(ifreq.ifr_name, name, IFNAMSIZ);

    if (ioctl(_udp_sock, SIOCGIFBRDADDR, &ifreq) != 0) {
        fprintf(stderr, "Could not find interface named %s", name);
        return INADDR_ANY;
    }

    getnameinfo(&ifreq.ifr_broadaddr, sizeof(ifreq.ifr_broadaddr), host, sizeof(host), 0, 0, NI_NUMERICHOST);
    return inet_addr(host);
}

//-----------------------------------------------------------------------------
static void
_prepare_dst_addr(void) {
    in_addr_t res;
    struct in_addr in;

    const char *_addr_str = getenv("VS_BCAST_SUBNET_ADDR");
    if (!_addr_str) {

        res = _get_interface_bcast_addr(_defaultNetif.c_str());
        if (INADDR_ANY != res) {
            _dst_addr = res;
        } else {

            res = _get_interface_bcast_addr("wlan0");
            if (INADDR_ANY != res) {
                _dst_addr = res;
            } else {

                _dst_addr = INADDR_BROADCAST;
            }
        }
    } else {
        _dst_addr = inet_addr(_addr_str);
    }

    in.s_addr = _dst_addr;
    VS_LOG_INFO("VS_BCAST_SUBNET_ADDR = %s", inet_ntoa(in));
}

//-----------------------------------------------------------------------------
static vs_status_e
_udp_connect() {
    struct sockaddr_in server;
    struct timeval tv;
    int enable = 1;

    // Create socket
    _udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (_udp_sock == -1) {
        VS_LOG_ERROR("UDP Broadcast: Could not create socket. %s\n", strerror(errno));
        return VS_CODE_ERR_SOCKET;
    }

    // Set infinite timeout
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    if (setsockopt(_udp_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        VS_LOG_ERROR("UDP Broadcast: Cannot set infinite timeout on receive. %s\n", strerror(errno));
        goto terminate;
    }

#if _APPLE_
    // Set SO_REUSEPORT
    if (setsockopt(_udp_sock, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        VS_LOG_ERROR("UDP Broadcast: Cannot set SO_REUSEPORT. %s\n", strerror(errno));
        goto terminate;
    }
#else
    // Set SO_REUSEADDR
    if (setsockopt(_udp_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        printf("UDP Broadcast: Cannot set SO_REUSEADDR. %s\n", strerror(errno));
        goto terminate;
    }
#endif

    // Set SO_BROADCAST
    if (setsockopt(_udp_sock, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(int)) < 0) {
        VS_LOG_ERROR("UDP Broadcast: Cannot set SO_BROADCAST. %s\n", strerror(errno));
        goto terminate;
    }

    // Bind to port
    memset((void *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(UDP_BCAST_PORT);
    if (bind(_udp_sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0) {
        VS_LOG_ERROR("UDP Broadcast: UDP Broadcast: Bind error. %s\n", strerror(errno));
        goto terminate;
    }

    VS_LOG_INFO("Opened connection for UDP broadcast\n");

    // Get broadcast address
    _prepare_dst_addr();

    // Start receive thread
    pthread_create(&receive_thread, NULL, _udp_receive_processor, NULL);

    return VS_CODE_OK;

    terminate:

    _udp_deinit(&_netif_udp_);

    return VS_CODE_ERR_SOCKET;
}

//-----------------------------------------------------------------------------
static bool
_netif_udp_internal(void) {
    // Get MAC address
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];
    bool success = false;
    bool res = false;

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    struct ifreq *it = ifc.ifc_req;
    const struct ifreq *const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

    CHECK(sock >= 0, "Cannot init UDP netif. Socket error, AF_INET::SOCK_DGRAM::IPPROTO_IP");
    CHECK(ioctl(sock, SIOCGIFCONF, &ifc) != -1, "Cannot init UDP netif. SIOCGIFCONF error");

    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        CHECK(ioctl(sock, SIOCGIFCONF, &ifc) == 0, "Cannot init UDP netif. SIOCGIFFLAGS error");

        if (!(ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
            if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                success = true;
                break;
            }
        }
    }

    if (success) {
        res = VS_CODE_OK == _udp_connect();
    }

    terminate:

    if (sock >= 0) {
        close(sock);
    }
    return res;
}

//-----------------------------------------------------------------------------
static void *
_udp_connection_processor(void *) {
    vs_log_thread_descriptor("udp conn thr");

    while (!_ready) {
        // Wait for a connection
        if (!_netif_udp_internal()) {
            usleep(1000 * 1000);
            continue;
        } else {
            _connecting = false;
            _ready = true;
            VS_LOG_DEBUG("UDP connection is activated");
        }
    }

    return NULL;
}

//-----------------------------------------------------------------------------
static vs_status_e
_internal_udp_tx(const uint8_t *data, const uint16_t data_sz) {
    // TODO: ARP request by DST mac address

    struct sockaddr_in broadcast_addr;

    memset((void *)&broadcast_addr, 0, sizeof(struct sockaddr_in));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = _dst_addr;
    broadcast_addr.sin_port = htons(UDP_BCAST_PORT);

    sendto(_udp_sock, data, data_sz, 0, (struct sockaddr *)&broadcast_addr, sizeof(struct sockaddr_in));

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_udp_tx(struct vs_netif_t *netif, const uint8_t *data, const uint16_t data_sz) {
    (void)netif;

    if (!_ready) {
        return VS_CODE_OK;
    }

    if (VS_CODE_OK == _internal_udp_tx(data, data_sz)) {
        vs_snap_packet_dump("OUT", (vs_snap_packet_t *)data);
        //--------------------------------------------
        //  QoS 1
        //--------------------------------------------
        vs_snap_packet_t *p = (vs_snap_packet_t *)data;
        if (0 != memcmp(p->eth_header.dest.bytes, vs_snap_broadcast_mac()->bytes, ETH_ADDR_LEN)) {
            _resendContainer->addPacket(&p->eth_header.dest, p->header.transaction_id, data, data_sz);
        }
        //--------------------------------------------
        //  ~QoS 1
        //--------------------------------------------
    }

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_udp_init(struct vs_netif_t *netif, const vs_netif_rx_cb_t rx_cb, const vs_netif_process_cb_t process_cb) {
    assert(rx_cb);
    (void)netif;

    if (!_resendContainer) {
        _resendContainer = new KSResendContainer(_internal_udp_tx);
    }

    _netif_udp_rx_cb = rx_cb;
    _netif_udp_process_cb = process_cb;
    _netif_udp_.packet_buf_filled = 0;


    if (!_ready && !_connecting) {
        VS_LOG_DEBUG("Start connection thread");
        _connecting = true;
        pthread_create(&connect_thread, NULL, _udp_connection_processor, NULL);
    }

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_udp_deinit(struct vs_netif_t *netif) {
    if (!_ready) {
        return VS_CODE_OK;
    }
    (void)netif;
    if (_udp_sock >= 0) {
#if !defined(_APPLE_)
        shutdown(_udp_sock, SHUT_RDWR);
#endif
        close(_udp_sock);
    }
    _udp_sock = -1;
    pthread_join(receive_thread, NULL);
    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_udp_mac(const struct vs_netif_t *netif, struct vs_mac_addr_t *mac_addr) {
    (void)netif;

    if (mac_addr) {
        memcpy(mac_addr->bytes, _mac_addr, sizeof(vs_mac_addr_t));
        return VS_CODE_OK;
    }

    return VS_CODE_ERR_NULLPTR_ARGUMENT;
}

//-----------------------------------------------------------------------------
static void
_update_mac(void) {
    std::ifstream iface("/sys/class/net/" + _defaultNetif + "/address");
    std::string str((std::istreambuf_iterator<char>(iface)), std::istreambuf_iterator<char>());
    if (str.length() > 0) {
        unsigned int bytes[ETH_ADDR_LEN];
        if (std::sscanf(str.c_str(),
                        "%02x:%02x:%02x:%02x:%02x:%02x",
                        &bytes[0],
                        &bytes[1],
                        &bytes[2],
                        &bytes[3],
                        &bytes[4],
                        &bytes[5]) != ETH_ADDR_LEN) {
            throw std::runtime_error(std::string("Invalid MAC address"));
        }

        for (int i = 0; i < ETH_ADDR_LEN; i++) {
            _mac_addr[i] = bytes[i];
        }
    }
}

//-----------------------------------------------------------------------------
extern "C" vs_netif_t *
vs_hal_netif_udp(void) {
    _update_mac();
    return &_netif_udp_;
}

//-----------------------------------------------------------------------------
