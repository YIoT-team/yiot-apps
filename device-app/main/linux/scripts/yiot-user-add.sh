#!/bin/bash

echo ">>> START: user add"

echo "SCRIPT: ${0}"
echo "USER  : ${1}"
echo "PASS  : ${2}"

USER_NAME="${1}"
USER_PASSWORD="${2}"

if [ ! $(getent passwd ${USER_NAME}) ]; then
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


echo ">>> END: user add"
