#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $0 <app>"
    exit 1
fi
APP_NAME=${1}

source docker/dev/env.bash

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
        -it --rm \
        --net host \
        -e APP_NAME=${APP_NAME} \
        --name ${IMAGE_NAME}-instance ${DOCKER_IMAGE} 
else
    docker run \
        -v ${HOST_WORKDIR}:${DOCKER_WORKDIR} \
        -it --rm \
        --net host \
        -e APP_NAME=${APP_NAME} \
        --name ${IMAGE_NAME}-instance ${DOCKER_IMAGE} 
fi

