#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $0 <Unity bin path>"
    exit 1
fi
UNITY_BIN_PATH=${1}

export RESOLV_IPADDR=$(netstat -rn | grep '^0.0.0.0' | awk '{print $2}')
NETWORK_INTERFACE=$(netstat -rn | grep '^0.0.0.0' | awk '{print $NF}'| tr -d '\n')
export CORE_IPADDR=$(ip addr | grep inet | grep  "${NETWORK_INTERFACE}"  | awk '{print $2}' | awk -F/ '{print $1}')

if [ -z "${RESOLV_IPADDR}" ]
then
    echo "ERROR: can not found RESOLV_IPADDR"
    exit 1
fi
if [ -z "${CORE_IPADDR}" ]
then
    echo "ERROR: can not found CORE_IPADDR"
    exit 1
fi

if [ ! -f third-party/mustache/mo ]
then
    echo "ERROR: can not find third-party/mustache/mo"
    exit 1
fi

bash third-party/mustache/mo utils/template/core_config_json.mo > ${UNITY_BIN_PATH}/core_config.json

exit 0