#!/bin/bash
if [ $# -ne 1 ]
then
    echo "Usage: $0 <dev|run>"
    exit 1
fi
TYPE=${1}

CURRENT_DIR=$(dirname $0)

IMAGE_NAME=`cat ${CURRENT_DIR}/${TYPE}/image_name.txt`
IMAGE_TAG=`cat ${CURRENT_DIR}/${TYPE}/latest_version.txt`
DOCKER_IMAGE=ncesnagoya/${IMAGE_NAME}:${IMAGE_TAG}

echo Loading ${IMAGE_NAME}-${IMAGE_TAG}.tar ...
docker load -i ${IMAGE_NAME}-${IMAGE_TAG}.tar