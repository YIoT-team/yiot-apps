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

#include <arpa/inet.h>
#include <assert.h>
#include <stddef.h>

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <virgil/iot/protocols/snap.h>
#include <virgil/iot/protocols/snap/snap-structs.h>

#include "iotkit-impl/netif/netif-ble-linux.h"

#include <bluez-dbus-cpp/bluez.h>
#include <bluez-dbus-cpp/GenericCharacteristic.h>
#include <bluez-dbus-cpp/ReadOnlyCharacteristic.h>

#include <signal.h>

using namespace org::bluez;
using namespace VirgilIoTKit;

constexpr const char *BLUEZ_SERVICE = "org.bluez";
constexpr const char *DEVICE0 = "/org/bluez/hci0";
constexpr const char *IOTKIT_BLE_SERVICE_UUID = "0000abf0-0000-1000-8000-00805f9b34fb";
constexpr const char *IOTKIT_BLE_CHAR_TX_UUID = "0000abf2-0000-1000-8000-00805f9b34fb";
constexpr const char *IOTKIT_BLE_CHAR_RX_UUID = "0000abf1-0000-1000-8000-00805f9b34fb";

static std::thread *_ble_thread = nullptr;

static std::mutex _mtx_start;
static std::condition_variable _cv_start;
static std::mutex _mtx_stop;
static std::condition_variable _cv_stop;
static bool _ready = false;
static bool _need_stop = false;
static vs_mac_addr_t _mac;

static vs_netif_rx_cb_t _netif_ble_rx_cb = 0;
static vs_netif_process_cb_t _netif_ble_process_cb = 0;

class TxCharacteristic;
static std::shared_ptr<TxCharacteristic> _tx_char;
static std::string NAME;

//-----------------------------------------------------------------------------
static vs_status_e
_ble_init(struct vs_netif_t *netif, const vs_netif_rx_cb_t rx_cb, const vs_netif_process_cb_t process_cb);

static vs_status_e
_ble_deinit(struct vs_netif_t *netif);

static vs_status_e
_ble_tx(struct vs_netif_t *netif, const uint8_t *data, const uint16_t data_sz);

static vs_status_e
_ble_mac(const struct vs_netif_t *netif, struct vs_mac_addr_t *mac_addr);

static vs_netif_t _netif_ble = {.user_data = NULL,
                                .init = _ble_init,
                                .deinit = _ble_deinit,
                                .tx = _ble_tx,
                                .mac_addr = _ble_mac,
                                .packet_buf_filled = 0};

//-----------------------------------------------------------------------------
class RxCharacteristic : public GattCharacteristicBuilder<GenericCharacteristic> {
public:
    RxCharacteristic(std::shared_ptr<GattService1> service, std::string uuid)
        : GattCharacteristicBuilder{move(service), move(uuid)} {
        flags_ = {"read", "write", "indicate", "write-without-response"};
    }

    static RxCharacteristic &
    create(std::shared_ptr<GattService1> service, std::string uuid) {
        auto self = new RxCharacteristic(move(service), move(uuid));
        return *self;
    }

protected:
    virtual void
    WriteValue(const std::vector<uint8_t> &value, const std::map<std::string, sdbus::Variant> &options) {
        // TODO handle options
        value_ = value;

        const uint8_t *packet_data = NULL;
        uint16_t packet_data_sz = 0;
        const uint8_t *rx_buffer = value.data();
        uint16_t recv_size = value.size();

        VS_LOG_DEBUG("Received %d bytes:", recv_size);

        if (recv_size > 1) {
            uint8_t marker = rx_buffer[0];
            if (marker != 0xAA && marker != 0x55) {
                VS_LOG_WARNING("Malformed packet");
                return;
            }

            if (marker == 0xAA) {
                // First element of packet. Need to reset received data.
                _netif_ble.packet_buf_filled = 0;
            }

            if (_netif_ble_rx_cb) {
                if (0 == _netif_ble_rx_cb(&_netif_ble, &rx_buffer[1], recv_size - 1, &packet_data, &packet_data_sz)) {
                    // Ready to process packet
                    if (_netif_ble_process_cb) {
                        VS_LOG_HEX(VS_LOGLEV_DEBUG, "RECV DUMP:", packet_data, packet_data_sz);
                        _netif_ble_process_cb(&_netif_ble, packet_data, packet_data_sz);
                    }
                }
            }
        }
    }
};

//-----------------------------------------------------------------------------
class TxCharacteristic : public GattCharacteristicBuilder<GenericCharacteristic> {
public:
    TxCharacteristic(std::shared_ptr<GattService1> service, std::string uuid)
        : GattCharacteristicBuilder{move(service), move(uuid)} {
        flags_ = {"read", "write", "notify"};
    }

    void
    tx(const std::vector<uint8_t> &value) {
        value_ = value;
        emitPropertyChangedSignal("Value");
    }
};

//-----------------------------------------------------------------------------
static void
_ble_thread_internal_func() {
    constexpr const char *APP_PATH = "/com/kutashenko/provision";
    constexpr const char *SRV_PATH = "/com/kutashenko/provision/service1";
    constexpr const char *ADV_PATH = "/com/kutashenko/provision/advertisement1";

    std::shared_ptr<IConnection> connection{std::move(sdbus::createSystemBusConnection())};

    // ---- Adapter Info -----------------------------------------------------------------------------------------------

    {
        Adapter1 adapter1{*connection, BLUEZ_SERVICE, DEVICE0};

        adapter1.Powered(true);
        adapter1.Discoverable(true);
        adapter1.DiscoverableTimeout(0);
        adapter1.Pairable(true);

        NAME = "yiot_RPi_" + adapter1.Address();

        // Save MAC address
        unsigned int bytes[ETH_ADDR_LEN];
        if (std::sscanf(adapter1.Address().c_str(),
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
            _mac.bytes[i] = bytes[i];
        }

        adapter1.Alias(NAME);

        std::cout << "Found adapter '" << DEVICE0 << "'" << std::endl;
        std::cout << "  Name: " << adapter1.Name() << std::endl;
        std::cout << "  Address: " << adapter1.Address() << " type: " << adapter1.AddressType() << std::endl;
        std::cout << "  Powered: " << adapter1.Powered() << std::endl;
        std::cout << "  Discoverable: " << adapter1.Discoverable() << std::endl;
        std::cout << "  Pairable: " << adapter1.Pairable() << std::endl;
    }

    std::cout << std::endl;

    // ---- Services ---------------------------------------------------------------------------------------------------
    GattManager1 gattMgr{connection, BLUEZ_SERVICE, DEVICE0};
    auto app = std::make_shared<GattApplication1>(connection, "/org/bluez/service");
    auto srv1 = std::make_shared<GattService1>(app, "deviceinfo", "180A");
    ReadOnlyCharacteristic::createFinal(srv1, "2A24", NAME);               // model name
    ReadOnlyCharacteristic::createFinal(srv1, "2A25", "000-11111111-222"); // serial number
    ReadOnlyCharacteristic::createFinal(srv1, "2A26", "0.0.1");            // fw rev
    ReadOnlyCharacteristic::createFinal(srv1, "2A27", "rev A");            // hw rev
    ReadOnlyCharacteristic::createFinal(srv1, "2A28", "1.0");              // sw rev
    ReadOnlyCharacteristic::createFinal(srv1, "2A29", "Kutashenko PE");    // manufacturer

    auto srv_iotkit = std::make_shared<GattService1>(app, "IoTKit", IOTKIT_BLE_SERVICE_UUID);
    _tx_char = std::shared_ptr<TxCharacteristic>(new TxCharacteristic(srv_iotkit, IOTKIT_BLE_CHAR_TX_UUID));
    _tx_char->finalize();
    RxCharacteristic::create(srv_iotkit, IOTKIT_BLE_CHAR_RX_UUID).finalize();

    auto register_app_callback = [](const sdbus::Error *error) {
        if (error == nullptr) {
            std::cout << "Application registered." << std::endl;
        } else {
            std::cerr << "Error registering application " << error->getName() << " with message " << error->getMessage()
                      << std::endl;
        }
    };

    gattMgr.RegisterApplicationAsync(app->getPath(), {}).uponReplyInvoke(register_app_callback);

    // ---- Advertising ------------------------------------------------------------------------------------------------

    auto mgr = std::make_shared<LEAdvertisingManager1>(connection, BLUEZ_SERVICE, DEVICE0);
    std::cout << "LEAdvertisingManager1" << std::endl;
    std::cout << "  ActiveInstances: " << std::to_string(mgr->ActiveInstances()) << std::endl;
    std::cout << "  SupportedInstances: " << std::to_string(mgr->SupportedInstances()) << std::endl;
    {
        std::cout << "  SupportedIncludes: ";
        auto includes = mgr->SupportedIncludes();
        for (auto include : includes) {
            std::cout << "\"" << include << "\",";
        }
        std::cout << std::endl;
    }

    auto register_adv_callback = [](const sdbus::Error *error) -> void {
        if (error == nullptr) {
            std::cout << "Advertisement registered." << std::endl;
        } else {
            std::cerr << "Error registering advertisment " << error->getName() << " with message "
                      << error->getMessage() << std::endl;
        }

        // Inform about readiness of BLE
        std::unique_lock<std::mutex> lck(_mtx_start);
        _ready = true;
        _cv_start.notify_one();
    };

    auto ad = LEAdvertisement1::create(*connection, ADV_PATH)
                      .withLocalName(NAME)
                      .withServiceUUIDs(std::vector{std::string{IOTKIT_BLE_SERVICE_UUID}})
                      .withIncludes(std::vector{std::string{"tx-power"}})
                      .onReleaseCall([]() { std::cout << "advertisement released" << std::endl; })
                      .registerWith(mgr, register_adv_callback);

    std::cout << "Loading complete." << std::endl;

    connection->enterEventLoopAsync();

    // Wait to finish
    std::unique_lock<std::mutex> lck(_mtx_stop);
    _cv_stop.wait(lck, []() -> bool { return _need_stop; });
}
//-----------------------------------------------------------------------------
static void
_ble_thread_func() {
    while (!_need_stop) {
        try {
            _ble_thread_internal_func();
        } catch (...) {
            if (!_need_stop) {
                usleep(3 * 1000 * 1000);
            }
        }
    }
}

//-----------------------------------------------------------------------------
static vs_status_e
_ble_connect() {
    if (_ble_thread) {
        // TODO: Stop
    }

    _ready = false;
    _need_stop = false;
    _ble_thread = new std::thread(_ble_thread_func);

    // Wait for BLE started
    std::unique_lock<std::mutex> lck(_mtx_start);
    _cv_start.wait(lck, []() -> bool { return _ready; });
    VS_LOG_DEBUG("BLE NETIF is connected");

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_ble_tx(struct vs_netif_t *netif, const uint8_t *data, const uint16_t data_sz) {
    (void)netif;
    if (!_tx_char.get()) {
        return VS_CODE_ERR_NOINIT;
    }

    vs_snap_packet_dump("OUT", (vs_snap_packet_t *)data);
    std::vector<uint8_t> data_vect(data, data + data_sz);
    _tx_char->tx(data_vect);

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_ble_init(struct vs_netif_t *netif, const vs_netif_rx_cb_t rx_cb, const vs_netif_process_cb_t process_cb) {
    assert(rx_cb);
    (void)netif;
    _netif_ble_rx_cb = rx_cb;
    _netif_ble_process_cb = process_cb;
    _netif_ble.packet_buf_filled = 0;
    _ble_connect();

    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_ble_deinit(struct vs_netif_t *netif) {
    (void)netif;
    if (_ble_thread) {
        {
            std::unique_lock<std::mutex> lck(_mtx_stop);
            _need_stop = true;
        }
        _cv_stop.notify_one();
        _ble_thread->join();
        delete _ble_thread;
        _ble_thread = nullptr;
    }
    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
static vs_status_e
_ble_mac(const struct vs_netif_t *netif, struct vs_mac_addr_t *mac_addr) {
    (void)netif;
    VS_IOT_MEMCPY(mac_addr->bytes, _mac.bytes, ETH_ADDR_LEN);
    return VS_CODE_OK;
}

//-----------------------------------------------------------------------------
vs_netif_t *
ks_netif_ble(void) {
    return &_netif_ble;
}

//-----------------------------------------------------------------------------
