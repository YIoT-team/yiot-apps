#!/bin/bash

set -e

echo ">>> START: RPi initialization"

echo "SCRIPT     : ${0}"
echo "INTERFACE  : ${1}"
echo "MODE       : ${2}"
echo "IPv4       : ${3}"
echo "GW         : ${4}"
echo "DNS        : ${5}"

mkdir -p /etc/yiot/network || true

NET_INTERFACE="${1}"
NET_MODE="${2}"
NET_IPADDR="${3}"
NET_GATEWAY="${4}"
NET_DNS="${5}"

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
	echo                                          > /etc/yiot/network/${NET_INTERFACE}
	echo "interface ${NET_INTERFACE}"            >> /etc/yiot/network/${NET_INTERFACE}
	echo "static ip_address=${NET_IPADDR}"       >> /etc/yiot/network/${NET_INTERFACE}
	echo "static routers=${NET_GATEWAY}"         >> /etc/yiot/network/${NET_INTERFACE}
	echo "static domain_name_servers=${NET_DNS}" >> /etc/yiot/network/${NET_INTERFACE}
	echo                                         >> /etc/yiot/network/${NET_INTERFACE}
    fi
    [ "$(ls -A /etc/yiot/network)" ] && cat /etc/yiot/network/* >> /etc/dhcpcd.conf || true
}

restart_network() {
    ip addr flush dev ${NET_INTERFACE}
    systemctl restart dhcpcd.service
}

restore_network_original
create_network_config
restart_network

echo ">>> END  : RPi initialization"
