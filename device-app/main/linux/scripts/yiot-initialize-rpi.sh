#!/bin/bash

echo ">>> START: RPi initialization"

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

sleep 4s

echo ">>> END  : RPi initialization"
