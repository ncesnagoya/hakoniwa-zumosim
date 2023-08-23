#!/bin/bash

MQTT_PORT=
echo "INFO: ACTIVATING HAKO-CONDUCTOR"
hakoniwa-conductor ${DELTA_MSEC} ${MAX_DELAY_MSEC} ${CORE_IPADDR}:${GRPC_PORT} ${UDP_SRV_PORT} ${UDP_SND_PORT} ${MQTT_PORT} & 

sleep 1

echo "INFO: ACTIVATING ZUMOSIM-RUNNER"
cmake-build/src/runner/zumosim-runner zumosim-runner ./runtime/custom.json ${DELTA_MSEC} 
