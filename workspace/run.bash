#!/bin/bash

ASSET_DEF=asset_def.txt
grep mqtt ${ASSET_DEF} > /dev/null
HAS_MQTT=$?
echo "ASSET_DEF=${ASSET_DEF}"
if [ ${HAS_MQTT} -eq 0 ]
then
    echo "INFO: ACTIVATING MOSQUITTO"
    mosquitto -c ./config/mosquitto.conf &
    MQTT_PORT=1883
    sleep 2
else
    MQTT_PORT=
fi


echo "INFO: ACTIVATING HAKO-CONDUCTOR"
hakoniwa-conductor ${DELTA_MSEC} ${MAX_DELAY_MSEC} ${CORE_IPADDR}:${GRPC_PORT} ${UDP_SRV_PORT} ${UDP_SND_PORT} ${MQTT_PORT} & 

sleep 1

echo "INFO: ACTIVATING ZUMOSIM-RUNNER"
cmake-build/src/runner/zumosim-runner zumosim-runner ./runtime/custom.json ${DELTA_MSEC} 
