#!/bin/bash

echo ">>> START: RPi initialization"

echo "SCRIPT: ${0}"
echo "USER  : ${1}"
echo "PASS  : ${2}"
echo "IPv4  : ${3}"
echo "GW    : ${4}"
echo "DNS   : ${5}"


USER_NAME="${1}"
USER_PASSWORD="${2}"
NETV4_IPADDR="${3}"
NETV4_GATEWAY="${4}"
NETV4_DNS="${5}"


if [ ! $(getent passwd jenkins) ]; then
  echo "Add user ${USER_NAME}"
  if [ ! -z "${USER_NAME}" ]; then
    useradd "${USER_NAME}"
    RET_RES="${?}"
    if [ "${RET_RES}" != "0" ]; then
      echo "Errot add user ERR:[${RET_RES}]"
      exit "${RET_RES}"
    fi
  fi 
fi  

if [ ! -z "${USER_PASSWORD}" ]; then
  echo "Change password ${USER_NAME}"
  echo "${USER_NAME}:${USER_PASSWORD}" | chpasswd
  RET_RES="${?}"
  if [ "${RET_RES}" != "0" ]; then
    echo "Errot change password ERR:[${RET_RES}]"
    exit "${RET_RES}"
  fi
fi

echo "Add user to sudoers"
echo "${USER_NAME}    ALL=(ALL:ALL) ALL" > /etc/sudoers.d/${USER_NAME}

echo "Setting  ip_address=${NETV4_IPADDR}"
if [ ! -f /etc/dhcpcd.conf.orig ]; then
  cp -f /etc/dhcpcd.conf /etc/dhcpcd.conf.orig
fi
cp -f /etc/dhcpcd.conf.orig /etc/dhcpcd.conf

echo >> /etc/dhcpcd.conf 
echo "interface wlan0" >> /etc/dhcpcd.conf
echo "static ip_address=${NETV4_IPADDR}" >> /etc/dhcpcd.conf
echo "static routers=${NETV4_GATEWAY}" >> /etc/dhcpcd.conf
echo "static domain_name_servers=${NETV4_DNS}" >> /etc/dhcpcd.conf

echo "Restarting dhcpcd.service"
ip addr flush dev wlan0
systemctl restart dhcpcd.service

echo ">>> END  : RPi initialization"
