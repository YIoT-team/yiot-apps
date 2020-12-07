#!/bin/bash

echo ">>> START: Setup RPi as VPN router"

echo "SCRIPT: ${0}"
echo "USER  : ${1}"
echo "PASS  : ${2}"
echo "IPv4  : ${3}"

sleep 2s
echo "Setup USER"

sleep 2s
echo "Setup STATIC IP"

sleep 2s
echo "Setup SSH"

sleep 2s
echo "Setup VPN"

sleep 4s

echo ">>> END  : Setup RPi as VPN router"
