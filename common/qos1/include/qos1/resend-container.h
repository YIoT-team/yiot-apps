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


#ifndef YIOT_RESEND_CONTAINER_H
#define YIOT_RESEND_CONTAINER_H

#include <list>
#include <vector>
#include <chrono>
#include <mutex>
#include <functional>

#include <stddef.h>
#include <stdint.h>

#include <virgil/iot/protocols/snap/snap-structs.h>

using namespace VirgilIoTKit;

class KSResendContainer {

    struct KSMessage {
        vs_mac_addr_t mac;
        vs_snap_transaction_id_t id;
        std::vector<uint8_t> data;
        size_t attempts;
        std::chrono::time_point<std::chrono::system_clock> time;
    };

public:
    KSResendContainer(std::function<vs_status_e(const uint8_t *, const uint16_t)> senderFunc,
                      size_t resendAttempts = 5,
                      size_t maxSz = 100,
                      std::chrono::milliseconds timeQuant = std::chrono::milliseconds(1000));
    virtual ~KSResendContainer() = default;

    bool
    addPacket(const vs_mac_addr_t *mac, vs_snap_transaction_id_t id, const uint8_t *data, size_t data_sz);

    void
    processResponse(const vs_mac_addr_t *mac, vs_snap_transaction_id_t id);

    //    void
    //    process();

private:
    static const size_t kSzLimit = 1000;
    static const size_t kResendLimit = 10;
    static constexpr std::chrono::milliseconds kQuantMinLimit = std::chrono::milliseconds(100);
    static constexpr std::chrono::milliseconds kQuantMaxLimit = std::chrono::milliseconds(10000);
    size_t m_szMax;
    size_t m_resendMax;
    std::chrono::milliseconds m_quant;
    std::list<KSMessage> m_container;
    std::mutex m_guard;
    std::function<vs_status_e(const uint8_t *, const uint16_t)> m_senderFunc;

    bool
    cleanQueue();
};

#endif // YIOT_RESEND_CONTAINER_H
