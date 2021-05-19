#!/bin/bash

#  ────────────────────────────────────────────────────────────
#                     ╔╗  ╔╗ ╔══╗      ╔════╗
#                     ║╚╗╔╝║ ╚╣╠╝      ║╔╗╔╗║
#                     ╚╗╚╝╔╝  ║║  ╔══╗ ╚╝║║╚╝
#                      ╚╗╔╝   ║║  ║╔╗║   ║║
#                       ║║   ╔╣╠╗ ║╚╝║   ║║
#                       ╚╝   ╚══╝ ╚══╝   ╚╝
#    ╔╗╔═╗                    ╔╗                     ╔╗
#    ║║║╔╝                   ╔╝╚╗                    ║║
#    ║╚╝╝  ╔══╗ ╔══╗ ╔══╗  ╔╗╚╗╔╝  ╔══╗ ╔╗ ╔╗╔╗ ╔══╗ ║║  ╔══╗
#    ║╔╗║  ║║═╣ ║║═╣ ║╔╗║  ╠╣ ║║   ║ ═╣ ╠╣ ║╚╝║ ║╔╗║ ║║  ║║═╣
#    ║║║╚╗ ║║═╣ ║║═╣ ║╚╝║  ║║ ║╚╗  ╠═ ║ ║║ ║║║║ ║╚╝║ ║╚╗ ║║═╣
#    ╚╝╚═╝ ╚══╝ ╚══╝ ║╔═╝  ╚╝ ╚═╝  ╚══╝ ╚╝ ╚╩╩╝ ║╔═╝ ╚═╝ ╚══╝
#                    ║║                         ║║
#                    ╚╝                         ╚╝
#
#    Lead Maintainer:
#  ────────────────────────────────────────────────────────────

SCRIPT_FOLDER="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_SERVER="8.8.8.8"
TIMEOUT=30

# -----------------------------------------------------------------------------

echo "Setup VPN WiFi bridge: "

AP_NAME="${1}"
AP_PASS="${2}"
PROVIDER="${3}"
VPN_USER="${4}"
VPN_PASS="${5}"

echo "SCRIPT     : ${0}"
echo "AP_NAME    : ${AP_NAME}"
echo "AP_PASS    : ${AP_PASS}"
echo "PROVIDER   : ${PROVIDER}"
echo "USER       : ${VPN_USER}"
echo "PASS       : ${VPN_PASS}"

wait_connection() {
    PING_IP="${1}"
    PING_TIMEOUT="${2}"

    echo -n "Wait for ${PING_IP} ..."
    let "end_time=$(date +%s) + PING_TIMEOUT"

    while :; do
        if ! ping -c 1 -W 1 ${PING_IP}; then
            if [ "$(date +%s)" -gt "$end_time" ]; then
                echo "ERROR TIMEOUT"
                return 128
            fi
            sleep 1
            echo -n "."
        else
            return 0
        fi
    done
}

if [ ! -f /etc/debian_version ]; then
    echo "ERROR: System is not Raspberry PI"
    exit 0
fi

echo "Setup STA/AP bridge"
${SCRIPT_FOLDER}/setup-access-point.sh down || true
${SCRIPT_FOLDER}/setup-access-point.sh up -essid "${AP_NAME}" -wpa "${AP_PASS}"

echo "Download OVPN configuration"
cd /tmp && wget https://files.ovpn.com/raspbian/ovpn-se-gothenburg.zip &&
    unzip ovpn-se-gothenburg.zip &&
    mkdir -p /etc/openvpn &&
    mv config/* /etc/openvpn &&
    chmod +x /etc/openvpn/update-resolv-conf &&
    rm -rf config &&
    rm -f ovpn-se-gothenburg.zip

echo "Prepare credentials"
echo "${VPN_USER}" >>/etc/openvpn/credentials
echo "${VPN_PASS}" >>/etc/openvpn/credentials

echo "Add rules"
iptables -t nat -D POSTROUTING -o eth0 -j MASQUERADE
iptables -t nat -A POSTROUTING -o tun+ -j MASQUERADE
netfilter-persistent save

echo "Connect to VPN"
systemctl  enable openvpn-client@ovpn.service
systemctl  start openvpn-client@ovpn.service

# Check for connection
if ! wait_connection ${TEST_SERVER} ${TIMEOUT}; then
    echo "ERROR: Cannot connect. Restore original settings"
    systemctl  stop openvpn-client@ovpn.service
    systemctl  disable openvpn-client@ovpn.service
    iptables -t nat -D POSTROUTING -o eth0 -j MASQUERADE
    ${SCRIPT_FOLDER}/setup-access-point.sh down
    wait_connection ${TEST_SERVER} ${TIMEOUT}

    exit 128
fi

echo "Check connection"
sleep 10s
curl https://www.ovpn.com/v2/api/client/ptr | python -m json.tool

exit 0

# -----------------------------------------------------------------------------
