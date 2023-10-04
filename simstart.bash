#!/bin/bash
HAKO_UNITY_BIN_PATH="./ZumoApp"
HAKO_UNITY_BIN=model.exe


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

HAKO_RUN_PID=
HAKO_UNITY_PID=
DOCKER_ID=
IS_OCCURED_SIGEVENT="FALSE"
function signal_handler()
{
    IS_OCCURED_SIGEVENT="TRUE"
    echo "trapped"
}
function kill_process()
{
    echo "trapped"
    if [ -z "$HAKO_RUN_PID" ]
    then
        exit 0
    fi
    if [ ! -z "$HAKO_UNITY_PID" ]
    then
        echo "KILLING: UNITY $HAKO_UNITY_PID"
        taskkill.exe  /IM ${HAKO_UNITY_BIN} /F
        kill -9 $HAKO_UNITY_PID || echo "Failed to kill UNITY $HAKO_UNITY_PID"
    fi
    if [ ! -z "$DOCKER_ID" ]
    then
        echo "KILLING: docker $DOCKER_ID"
        docker kill ${DOCKER_ID}
    fi
    
    echo "KILLING: HAKO_RUN $HAKO_RUN_PID"
    kill -9 "$HAKO_RUN_PID" || echo "Failed to kill HAKO_RUN"

    while [ 1 ]
    do
        NUM=$(ps aux | grep "run.bash" | grep -v grep | wc -l)
        if [ $NUM -eq 0 ]
        then
            break
        fi
        sleep 1
    done

    exit 0
}
trap signal_handler SIGINT

echo "INFO: Simulatin start"

bash docker/run.bash no_tty &
HAKO_RUN_PID=$!

sleep 1

DOCKER_ID=`docker ps | grep "ncesnagoya/hakoniwa-suzmosim-run" | awk '{print $1}'`


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

