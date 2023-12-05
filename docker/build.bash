#!/bin/bash

if [ $# -ne 0 -a $# -ne 1 ]
then
    echo "Usage: $0 [<app>]"
    exit 1
fi
APP_NAME=
if [ $# -eq 1 ]
then
    APP_NAME=${1}
fi

source docker/dev/env.bash

if [ ! -z ${APP_NAME} ]
then
    if [ ! -d workspace/src/apl/apps/${APP_NAME} ]
    then
        echo "ERROR: can not found application name=${APP_NAME}  on workspace/src/apl/apps"
        exit 1
    fi
    rm -f workspace/src/apl/*.ino workspace/src/apl/*.h
    cp workspace/src/apl/apps/${APP_NAME}/*.ino workspace/src/apl/
    cp workspace/src/apl/apps/${APP_NAME}/*.h workspace/src/apl/
fi

HAKONIWA_TOP_DIR=`pwd`
IMAGE_NAME=`cat docker/dev/image_name.txt`
IMAGE_TAG=`cat docker/dev/latest_version.txt`
DOCKER_IMAGE=ncesnagoya/${IMAGE_NAME}:${IMAGE_TAG}

ARCH=`arch`
OS_TYPE=`bash docker/utils/detect_os_type.bash`
echo $ARCH
echo $OS_TYPE
if [ $ARCH = "arm64" ]
then
    docker run \
        --platform linux/amd64 \
        -v ${HOST_WORKDIR}:${DOCKER_WORKDIR} \
        -v ${HOST_ZUMO_INFDIR}:${DOCKER_ZUMO_INFDIR} \
        -it --rm \
        --net host \
        --name ${IMAGE_NAME}-instance ${DOCKER_IMAGE} ${DOCKER_RUN_CMD}
else
    docker run \
        -v ${HOST_WORKDIR}:${DOCKER_WORKDIR} \
        -v ${HOST_ZUMO_INFDIR}:${DOCKER_ZUMO_INFDIR} \
        -it --rm \
        --net host \
        --name ${IMAGE_NAME}-instance ${DOCKER_IMAGE} ${DOCKER_RUN_CMD}
fi

