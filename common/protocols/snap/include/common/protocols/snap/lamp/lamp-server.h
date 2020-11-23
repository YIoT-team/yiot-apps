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

#ifndef YIOT_SNAP_SERVICES_LAMP_SERVER_H
#define YIOT_SNAP_SERVICES_LAMP_SERVER_H

#if LAMP_SERVER

#include <virgil/iot/protocols/snap/snap-structs.h>
#include <virgil/iot/protocols/snap/msgr/msgr-structs.h>
#ifdef __cplusplus
namespace VirgilIoTKit {
extern "C" {
#endif

/** Get data from a device
 *
 * This function is called by receiving request to get actual data from a remote device or every periodical poll
 * processing.
 *
 * \param[out] State structure. Must not be NULL.
 *
 * \return #VS_CODE_OK in case of success or error code.
 */
typedef vs_status_e (*vs_snap_lamp_get_state_server_cb_t)(vs_snap_lamp_state_t *state);

/** Set data to a device
 *
 * This function is called by receiving request to set data to a remote device.
 *
 * \param[out] data Data to be saved. Must not be NULL.
 * \param[out] data_sz Data size. Must not be zero.
 *
 * \return #VS_CODE_OK in case of success or error code.
 */
typedef vs_status_e (*vs_snap_lamp_set_state_server_cb_t)(uint8_t *data, uint32_t data_sz);

/** LAMP server implementations
 *
 * \note Any callback can be NULL. In this case, there will be no actions with requests.
 *
 */
typedef struct {
    vs_snap_lamp_get_state_server_cb_t get_data; /**< Get data to send it over snap */
    vs_snap_lamp_set_state_server_cb_t set_data; /**< Set data received from snap*/
} vs_snap_lamp_server_service_t;

/** LAMP Server SNAP Service implementation
 *
 * This call returns LAMP server implementation. It must be called before any LAMP call.
 *
 * \param[in] impl Snap LAMP Server functions implementation.
 *
 * \return #vs_snap_service_t SNAP service description. Use this pointer to call #vs_snap_register_service.
 */
const vs_snap_service_t *
vs_snap_lamp_server(vs_snap_lamp_server_service_t impl);

#ifdef __cplusplus
} // extern "C"
} // namespace VirgilIoTKit
#endif

#endif // LAMP_SERVER

#endif // YIOT_SNAP_SERVICES_LAMP_SERVER_H
