#!/bin/bash
if [ $# -ne 1 ]
then
    echo "Usage: $0 <dev|run>"
    exit 1
fi
TYPE=${1}

HAKO_ARCHIVE=hakoniwa-conductor_Linux_x86_64_ubuntu-20.04.tar.gz

IMAGE_NAME=`cat docker/${TYPE}/image_name.txt`
IMAGE_TAG=`cat docker/${TYPE}/latest_version.txt`
DOCKER_IMAGE=ncesnagoya/${IMAGE_NAME}:${IMAGE_TAG}
if [ -f $HAKO_ARCHIVE ]; then
    DOCKER_FILE=docker/${TYPE}/Dockerfile.hako
else
    DOCKER_FILE=docker/${TYPE}/Dockerfile
fi

ARCH=`arch`
OS_TYPE=`bash docker/utils/detect_os_type.bash`
echo $ARCH
echo $OS_TYPE
if [ $ARCH = "arm64" ]
then
    docker build \
        --platform linux/amd64 \
        -t ${DOCKER_IMAGE} -f ${DOCKER_FILE} .
else
    docker build -t ${DOCKER_IMAGE} -f ${DOCKER_FILE} .
fi

if [ $TYPE = "dev" ]
then
    chmod +x workspace/build.bash
fi
