#!/bin/bash

echo "Configuring wpa_suplicant"

echo "SCRIPT: ${0}"
echo "SSID  : ${1}"
echo "PASS  : ${2}"

if [ ! -f /etc/debian_version ]; then
  echo "ERROR: System is not Raspberry PI"
  exit 0
fi

WIFI_ESSID="${1}"
WIFI_KEY="${2}"
TIMEOUT="45"

echo "Prepare WPA supplicant configuration"
cat <<EOF >/etc/wpa_supplicant/wpa_supplicant.conf
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=GB

EOF
wpa_passphrase "${WIFI_ESSID}" "${WIFI_KEY}" >> /etc/wpa_supplicant/wpa_supplicant.conf
sed -i '/#psk=/d' /etc/wpa_supplicant/wpa_supplicant.conf

echo "Unblock WiFi"
rfkill unblock wifi

echo "Enable WiFi interface"
ifconfig wlan0 up

echo "Restore network settings"
if [ -f /etc/dhcpcd.conf.orig ]; then
    cp -f /etc/dhcpcd.conf.orig /etc/dhcpcd.conf
fi

echo "Reconfiguring wpa_supplicant"
chmod 600 /etc/wpa_supplicant/wpa_supplicant.conf
wpa_cli -i wlan0 reconfigure

echo -n "Wait Wifi connection..."
let "end_time=$(date +%s) + TIMEOUT"
while :
 do
   sleep 1
   echo -n "."
   WIFI_RES=$(iwgetid |cut -d':' -f2| sed 's/"//g')
   if [ "${WIFI_RES}" == "${WIFI_ESSID}" ]; then
      echo -e "\nWiFi configured done"
      echo "Current ESSID: ${WIFI_RES}"
      break
   fi
   if [ "$(date +%s)" -gt "$end_time" ]; then
    echo "ERROR TIMEOUT"
    exit 128
   fi
done

exit 0
