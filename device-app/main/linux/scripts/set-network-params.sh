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

set -e

echo "START: Network settings"

echo "SCRIPT     : ${0}"
echo "INTERFACE  : ${1}"
echo "STATIC     : ${2}"
echo "IP         : ${3}"
echo "GATEWAY    : ${4}"
echo "DNS        : ${5}"
echo "MASK       : ${6}"

mkdir -p /etc/yiot/network || true

NET_INTERFACE="${1}"
NET_MODE="${2}"
NET_IPADDR="${3}"
NET_GATEWAY="${4}"
NET_DNS="${5}"
NET_MASK="${6}"
TIMEOUT=20

restore_network_original() {
    if [ ! -f /etc/dhcpcd.conf.orig ]; then
        echo "Backup original network settings file"
        cp -f /etc/dhcpcd.conf /etc/dhcpcd.conf.orig
    fi
    echo "Restore original network settings file"
    cp -f /etc/dhcpcd.conf.orig /etc/dhcpcd.conf
}

create_network_config() {
    if [ "${NET_MODE}" == "dhcp" ]; then
        echo "Remove static IP settings file"
        rm -f /etc/yiot/network/${NET_INTERFACE} || true
    else
        echo >/etc/yiot/network/${NET_INTERFACE}
        echo "interface ${NET_INTERFACE}" >>/etc/yiot/network/${NET_INTERFACE}
        echo "static ip_address=${NET_IPADDR}" >>/etc/yiot/network/${NET_INTERFACE}
        echo "static routers=${NET_GATEWAY}" >>/etc/yiot/network/${NET_INTERFACE}
        echo "static domain_name_servers=${NET_DNS}" >>/etc/yiot/network/${NET_INTERFACE}
        echo >>/etc/yiot/network/${NET_INTERFACE}
    fi
    [ "$(ls -A /etc/yiot/network)" ] && cat /etc/yiot/network/* >>/etc/dhcpcd.conf || true
}

restart_network() {
    ip addr flush dev ${NET_INTERFACE}
    systemctl restart dhcpcd.service
}

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

restore_network_original
create_network_config
restart_network

# Check for connection
if ! wait_connection ${NET_GATEWAY} ${TIMEOUT}; then
    echo "ERROR: Cannot connect. Restore original settings"
    restore_network_original
    restart_network
    wait_connection ${NET_GATEWAY} ${TIMEOUT}

    exit 128
fi

echo "END  : Network settings"
