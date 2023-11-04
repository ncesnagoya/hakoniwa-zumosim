#!/bin/bash
HAKO_UNITY_BIN_PATH="./ZumoApp"
HAKO_UNITY_BIN=model.exe


IMANGE_NAME=` cat docker/run/image_name.txt`
docker ps | grep "${IMANGE_NAME}" > /dev/null
if [ $? -ne 0 ]
then
    echo "ERROR: Cannot find hakoniwa-zumosim-run."
    echo "Please open a new terminal and execute the following command:"
    echo "bash docker/run.bash"
    exit 1
fi

if [ ! -d ${HAKO_UNITY_BIN_PATH} ]
then
    echo "ERROR: can not found ${HAKO_UNITY_BIN_PATH}"
    exit 1
fi
bash utils/set_core_config.bash ${HAKO_UNITY_BIN_PATH}
if [ $? -ne 0 ]
then
    exit 1
fi

echo "INFO: Unity start"




cd ${HAKO_UNITY_BIN_PATH}
./${HAKO_UNITY_BIN} &
HAKO_UNITY_PID=$!

sleep 1

while true; do
    echo "Press ENTER to stop..."
    read input
    if [ -z "$input" ]; then
        kill_process
        break
    fi
done

