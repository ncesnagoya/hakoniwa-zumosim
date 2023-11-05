#!/bin/bash

IMANGE_NAME=` cat docker/run/image_name.txt`
docker ps | grep "${IMANGE_NAME}" > /dev/null
if [ $? -ne 0 ]
then
    echo "INFO: Not find hakoniwa-zumosim-run."
    exit 1
fi

DOCKER_ID=`docker ps | grep "${IMANGE_NAME}" | awk '{print $1}'` 
docker kill ${DOCKER_ID}
