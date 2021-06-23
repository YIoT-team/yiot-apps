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

#include "common/helpers/timer.h"
#include <thread>

//-----------------------------------------------------------------------------
KSTimer::KSTimer() : m_running(false) {
    std::thread([=]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::lock_guard<std::mutex> lock(m_changingMutex);
            if (m_running && std::chrono::high_resolution_clock::now() - m_start > m_delay) {
                m_callback();

                std::lock_guard<std::mutex> lockState(m_stateMutex);
                m_running = false;
            }
        }
    }).detach();
}

//-----------------------------------------------------------------------------
bool
KSTimer::add(std::chrono::milliseconds delay, std::function<void()> callback) {
    std::lock_guard<std::mutex> lockState(m_stateMutex);
    if (m_running) {
        return false;
    }

    std::lock_guard<std::mutex> lock(m_changingMutex);
    m_callback = callback;
    m_start = std::chrono::high_resolution_clock::now();
    m_delay = delay;
    m_running = true;

    return true;
}

//-----------------------------------------------------------------------------
