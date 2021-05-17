#!/bin/bash

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

if [ ! -f /etc/debian_version ]; then
  echo "ERROR: System is not Raspberry PI"
  exit 0
fi

echo "Install required packages"

echo "Setup STA/AP bridge"


echo "Setup VPN connection"



exit 0
