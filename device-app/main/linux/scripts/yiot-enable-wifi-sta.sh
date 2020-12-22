#!/bin/bash

echo "Configuring wpa_suplicant"

echo "SCRIPT: ${0}"
echo "SSID  : ${1}"
echo "PASS  : ${2}"

WIFI_ESSID="${1}"
WIFI_KEY="${2}"
TIMEOUT="20"

cat <<EOF >/etc/wpa_supplicant/wpa_supplicant.conf
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=GB

network={
        ssid="${WIFI_ESSID}"
        psk="${WIFI_KEY}"
}
EOF

echo "Reconfiguring wpa_supplicant"
sudo wpa_cli -i wlan0 reconfigure

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
