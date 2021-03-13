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

#include <algorithm>
#include <thread>

#include "qos1/resend-container.h"

#if !defined(YIOT_DEBUG_QOS)
#define YIOT_DEBUG_QOS 0
#endif

//-----------------------------------------------------------------------------
KSResendContainer::KSResendContainer(std::function<vs_status_e(const uint8_t *, const uint16_t)> senderFunc,
                                     size_t resendAttempts,
                                     size_t maxSz,
                                     std::chrono::milliseconds timeQuant) {
    m_szMax = maxSz > kSzLimit ? kSzLimit : maxSz;
    m_resendMax = resendAttempts > kResendLimit ? kResendLimit : resendAttempts;

    if (timeQuant > kQuantMaxLimit) {
        m_quant = kQuantMaxLimit;
    } else if (timeQuant < kQuantMinLimit) {
        m_quant = kQuantMinLimit;
    } else {
        m_quant = timeQuant;
    }
    m_senderFunc = senderFunc;

    std::thread([=]() {
        while (true) {
            std::this_thread::sleep_for(timeQuant);
            std::lock_guard<std::mutex> lock(m_guard);
            for (auto &m : m_container) {
                // if (m.time - std::chrono::system_clock::now() >= timeQuant) {

                // Removed old packets
                auto end = std::remove_if(m_container.begin(), m_container.end(), [this](const KSMessage &m) {
                    return m.attempts > m_resendMax;
                });

                // Resend if required
                if (m.attempts < m_resendMax) {
                    if (m_senderFunc) {
#if YIOT_DEBUG_QOS
                        VS_LOG_DEBUG("Resend %u", (unsigned int)m.id);
#endif
                        m_senderFunc(m.data.data(), m.data.size());
                    }
                }

                ++m.attempts;

                //}
            }
        }
    })
            .detach();
}

//-----------------------------------------------------------------------------
bool
KSResendContainer::addPacket(const vs_mac_addr_t *mac,
                             vs_snap_transaction_id_t id,
                             const uint8_t *data,
                             size_t data_sz) {
    CHECK_NOT_ZERO_RET(mac, false);
    CHECK_NOT_ZERO_RET(data, false);
    CHECK_NOT_ZERO_RET(data_sz, false);

    std::lock_guard<std::mutex> l(m_guard);

    if (m_container.size() >= m_szMax) {
        VS_LOG_WARNING("Re-send container is full");
        return false;
    }

    KSMessage m;
    memcpy(m.mac.bytes, mac, ETH_ADDR_LEN);
    m.id = id;
    m.attempts = 1;
    std::vector<uint8_t> d(data, data + data_sz);
    m.time = std::chrono::system_clock::now();
    m.data.swap(d);

#if YIOT_DEBUG_QOS
    VS_LOG_DEBUG("Add packet %u", (unsigned int)id);
#endif

    m_container.push_back(m);

    return false;
}

//-----------------------------------------------------------------------------
void
KSResendContainer::processResponse(const vs_mac_addr_t *mac, vs_snap_transaction_id_t id) {
    std::lock_guard<std::mutex> l(m_guard);
    auto end = std::remove_if(m_container.begin(), m_container.end(), [mac, id](const KSMessage &m) {
        if (id == m.id) {
            if (0 == memcmp(mac->bytes, m.mac.bytes, ETH_ADDR_LEN)) {
#if YIOT_DEBUG_QOS
                VS_LOG_DEBUG("Packet processed %u", (unsigned int)id);
#endif
                return true;
            }
        }
        return false;
    });

    m_container.erase(end, m_container.end());
}

//-----------------------------------------------------------------------------
