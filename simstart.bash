#!/bin/bash
HAKO_UNITY_BIN_PATH="./ZumoApp"
HAKO_UNITY_BIN=model.exe
CURRENT_DIR=`pwd`

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
IS_OCCURED_SIGEVENT="FALSE"
function signal_handler()
{
    IS_OCCURED_SIGEVENT="TRUE"
    echo "trapped"
}
function kill_process()
{
    echo "INFO: STOPPING Simulation"
    if [ -z "$HAKO_RUN_PID" ]
    then
        exit 0
    fi
    cd $CURRENT_DIR
    bash docker/kill.bash
    if [ ! -z "$HAKO_UNITY_PID" ]
    then
        echo "KILLING: UNITY $HAKO_UNITY_PID"
        taskkill.exe  /IM ${HAKO_UNITY_BIN} /F
    fi

    while [ 1 ]
    do
        NUM=$(ps aux | grep "run.bash" | grep -v grep | wc -l)
        if [ $NUM -eq 0 ]
        then
            break
        fi
        sleep 1
    done
    echo "INFO: STOP DONE"
    exit 0
}
trap signal_handler SIGINT

echo "INFO: Simulatin start"

bash docker/run.bash no_tty &
HAKO_RUN_PID=$!

sleep 1

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

