#!/bin/bash
############################################################################
set -eE
ERR_STR=""

err_report() {
    trap - ERR
    echo "[ERROR]"
    echo "Error on line $1"
    echo "#----------------------"
    echo "${ERR_STR}"
    _down 2>&1 > /dev/null
    exit 127
}

print_stage() {
  echo -ne "--- ${@}..."
}

print_ok() {
  echo "[OK]"
}

trap 'err_report $LINENO' ERR
############################################################################
DEFAULT_IF_NAME="ap0"
DEFAULT_IF_MODE="g"
DEFAULT_IF_IPADDR="192.168.4.1"
DEFAULT_IF_IPMASK="255.255.255.0"
DEFAULT_IP_PREF="24"
DEFAULT_IP_RANGE_MIN="192.168.4.100"
DEFAULT_IP_RANGE_MAX="192.168.4.200"
DEFAULT_CHANNEL="7"
DEFAULT_ESSID="YioT"
DEFAULT_WPA="raspberry"

############################################################################
print_usage() {
    echo
    echo "$(basename ${0}) < up | down > <parameters>"

    echo
    echo "- Configure Access point:"
    echo "  $(basename ${0}) up"
    echo
    echo "  -mode   <Wireless mode a|b|g|n >  [default: [${DEFAULT_IF_MODE}]"
#    echo "  -ipaddr <IP addr>                 [default: [${DEFAULT_IF_IPADDR}]"
#    echo "  -mask   <IP mask>                 [default: [${DEFAULT_IF_IPMASK}]"
    echo "  -chan   <Wireless channel>        [default: [${DEFAULT_CHANNEL}]"
    echo "  -essid  <essid>                   [default: [${DEFAULT_ESSID}]"
    echo "  -wpa    <wpa key>                 [default: [${DEFAULT_WPA}]"
    echo "  -h    - Print usage"

    echo
    echo "- Shutdown Access point mode:"
    echo "  $(basename ${0}) down"
    exit 0
}

###########################################################################
PARAM_COMMAND="${1,,}"
if [ "${PARAM_COMMAND}" == "" ] || [ "${PARAM_COMMAND}" == "help" ]; then
    print_usage
    exit 0
fi

shift
while [ -n "$1" ]
do
    case "$1" in
        -h) print_usage
            ;;
        -mode)   DEFAULT_IF_MODE="$2"
                 shift
                 ;;
        -ipaddr) DEFAULT_IF_IPADDR="$2"
                 shift
                 ;;
        -mask)   DEFAULT_IF_IPMASK="$2"
                 shift
                 ;;
        -chan)   DEFAULT_CHANNEL="$2"
                 shift
                 ;;
        -essid)  DEFAULT_ESSID="$2"
                 shift
                 ;;
        -wpa)   DEFAULT_WPA="$2"
                 shift
                 ;;
        *)       print_usage
                 ;;
    esac
    shift
done

############################################################################
interface_ctl() {
   local PARAM_COMMAND="$1"
   if [ "${PARAM_COMMAND}" = "up" ]; then
      echo "ACTION==\"add\", SUBSYSTEM==\"ieee80211\", KERNEL==\"phy0\", \\"      >/etc/udev/rules.d/55-ap.rules
      echo "    RUN+=\"/usr/sbin/iw dev wlan0 interface add ap0 type __ap\", \\" >>/etc/udev/rules.d/55-ap.rules
      echo "    RUN+=\"/usr/sbin/ip link set ap0 up\""                           >>/etc/udev/rules.d/55-ap.rules
      rfkill unblock wlan
      iw dev wlan0 interface add "${DEFAULT_IF_NAME}" type __ap
      ip link set "${DEFAULT_IF_NAME}" up
      echo "net.ipv4.ip_forward=1" >>/etc/sysctl.d/ap.conf
      sysctl net.ipv4.ip_forward=1
   else
      rm -f /etc/sysctl.d/ap.conf
      rm -f /etc/udev/rules.d/55-ap.rules
      iw dev "${DEFAULT_IF_NAME}" del
   fi
}

############################################################################
restore_network_original() {
    if [ ! -f /etc/dhcpcd.conf.orig ]; then
      echo "Backup original network settings file"
      cp -f /etc/dhcpcd.conf /etc/dhcpcd.conf.orig
    fi
    echo "Restore original network settings file"
    cp -f /etc/dhcpcd.conf.orig /etc/dhcpcd.conf
}

############################################################################
configure_dhcpcd() {
   local PARAM_COMMAND="${1}"
   local PARAM_IP_ADDR="${2}"
   local PARAM_IP_PREF="${3}"
   mkdir -p /etc/yiot/network || true
   if [ "${PARAM_COMMAND}" = "up" ]; then
      restore_network_original
      echo "interface ${DEFAULT_IF_NAME}"                           > /etc/yiot/network/${DEFAULT_IF_NAME}
      echo "static ip_address=${PARAM_IP_ADDR}/${PARAM_IP_PREF}"   >> /etc/yiot/network/${DEFAULT_IF_NAME}
      echo "nohook wpa_supplicant"                                 >> /etc/yiot/network/${DEFAULT_IF_NAME}
      [ "$(ls -A /etc/yiot/network)" ] && cat /etc/yiot/network/*  >> /etc/dhcpcd.conf || true
   else
      rm -f /etc/yiot/network/${DEFAULT_IF_NAME}
      restore_network_original
      [ "$(ls -A /etc/yiot/network)" ] && cat /etc/yiot/network/* >> /etc/dhcpcd.conf || true
   fi
   systemctl restart dhcpcd.service
}

############################################################################
#  hw_mode=g
#    a = IEEE 802.11a (5 GHz) (Raspberry Pi 3B+ onwards)
#    b = IEEE 802.11b (2.4 GHz)
#    g = IEEE 802.11g (2.4 GHz)
############################################################
configure_hostap() {
   local PARAM_COMMAND="${1}"
   local PARAM_CHANNEL="${2}"
   local PARAM_MODE="${3}"
   local PARAM_ESSID="${4}"
   local PARAM_WPA="${5}"
   if [ "${PARAM_COMMAND}" = "up" ]; then
      echo "country_code=GB"               >/etc/hostapd/hostapd.conf
      echo "interface=${DEFAULT_IF_NAME}" >>/etc/hostapd/hostapd.conf
      echo "ssid=${PARAM_ESSID}"          >>/etc/hostapd/hostapd.conf
      echo "hw_mode=${PARAM_MODE}"        >>/etc/hostapd/hostapd.conf
      echo "channel=${PARAM_CHANNEL}"     >>/etc/hostapd/hostapd.conf
      echo "macaddr_acl=0"                >>/etc/hostapd/hostapd.conf
      echo "auth_algs=1"                  >>/etc/hostapd/hostapd.conf
      echo "ignore_broadcast_ssid=0"      >>/etc/hostapd/hostapd.conf
      echo "wpa=2"                        >>/etc/hostapd/hostapd.conf
      echo "wpa_passphrase=${PARAM_WPA}"  >>/etc/hostapd/hostapd.conf
      echo "wpa_key_mgmt=WPA-PSK"         >>/etc/hostapd/hostapd.conf
      echo "wpa_pairwise=TKIP"            >>/etc/hostapd/hostapd.conf
      echo "rsn_pairwise=CCMP"            >>/etc/hostapd/hostapd.conf
      systemctl unmask hostapd
      systemctl enable hostapd
      systemctl restart hostapd
   else
      systemctl stop hostapd
      systemctl disable hostapd
      systemctl mask hostapd
      rm -f /etc/hostapd/hostapd.conf
   fi
}

############################################################################
configure_dnsmask() {
   local PARAM_COMMAND="${1}"
   local PARAM_IP_GW="${2}"
   local PARAM_IP_MASK="${3}"
   local PARAM_IP_RANGE_MIN="${4}"
   local PARAM_IP_RANGE_MAX="${5}"
   if [ "${PARAM_COMMAND}" = "up" ]; then
      echo "interface=${DEFAULT_IF_NAME}" >/etc/dnsmasq.conf
      echo "dhcp-range=${PARAM_IP_RANGE_MIN},${PARAM_IP_RANGE_MAX},${PARAM_IP_MASK},24h" >>/etc/dnsmasq.conf
      echo "domain=wlan" >>/etc/dnsmasq.conf
      echo "address=/gw.wlan/${PARAM_IP_GW}" >>/etc/dnsmasq.conf
      systemctl enable dnsmasq.service
      systemctl restart dnsmasq.service
   else
      systemctl disable dnsmasq.service
      systemctl stop dnsmasq.service
   fi
}

############################################################################
configure_fw() {
   local PARAM_COMMAND="${1}"
   local PARAM_IP="${2}"
   if [ "${PARAM_COMMAND}" = "up" ]; then
      iptables -t nat -I POSTROUTING -o wlan0 -j MASQUERADE
      iptables -t nat -I POSTROUTING -o eth0 -j MASQUERADE
      netfilter-persistent save
   else
      iptables -t nat -D POSTROUTING -o wlan0 -j MASQUERADE
      iptables -t nat -D POSTROUTING -o eth0 -j MASQUERADE
      netfilter-persistent save
   fi
}

############################################################################
_down() {
   print_stage "Reset IP address"
   ERR_STR=$(configure_dhcpcd "down" 2>&1)
   print_ok

   print_stage "Shudown dnsmasq"
   ERR_STR=$(configure_dnsmask "down"  2>&1)
   print_ok

   print_stage "Shudown hostap"
   ERR_STR=$(configure_hostap "down"  2>&1)
   print_ok

   print_stage "Shudown Access point interface"
   ERR_STR=$(interface_ctl "down"  2>&1)
   print_ok

   print_stage "Reset firewall rules"
   ERR_STR=$(configure_fw "down" 2>&1)
   print_ok
}

############################################################################
_up() {
   print_stage "Set static IP address [${DEFAULT_IF_IPADDR}/${DEFAULT_IP_PREF}]"
   ERR_STR=$(configure_dhcpcd "up" ${DEFAULT_IF_IPADDR} ${DEFAULT_IP_PREF}  2>&1)
   print_ok

   print_stage "Starting interface ${DEFAULT_IF_NAME}"
   ERR_STR=$(interface_ctl "up"  2>&1)
   print_ok

   print_stage "Starting dnsmasq"
   ERR_STR=$(configure_dnsmask "up" ${DEFAULT_IF_IPADDR} ${DEFAULT_IF_IPMASK} ${DEFAULT_IP_RANGE_MIN} ${DEFAULT_IP_RANGE_MAX}  2>&1)
   print_ok

   print_stage "Starting hostap CHAN:[${DEFAULT_CHANNEL}] MODE:[${DEFAULT_IF_MODE}] ESSID:[${DEFAULT_ESSID}]"
   ERR_STR=$(configure_hostap "up" "${DEFAULT_CHANNEL}" "${DEFAULT_IF_MODE}" "${DEFAULT_ESSID}" "${DEFAULT_WPA}"  2>&1)
   print_ok

   print_stage "Setting firewall rules"
   ERR_STR=$(configure_fw "up" 2>&1)
   print_ok
}

############################################################################
case "${PARAM_COMMAND}" in
    up) _up
        ;;
  down) _down
        ;;
     *) echo "Error command"
        print_usage
        exit 127
        ;;
esac









