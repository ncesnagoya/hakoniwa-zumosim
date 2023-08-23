#!/bin/bash


HAKONIWA_TOP_DIR=`pwd`
IMAGE_NAME=`cat docker/run/image_name.txt`
IMAGE_TAG=`cat docker/run/latest_version.txt`
DOCKER_IMAGE=ncesnagoya/${IMAGE_NAME}:${IMAGE_TAG}

DOCKER_ID=`docker ps | grep "${DOCKER_IMAGE}" | awk '{print $1}'`

docker exec -it ${DOCKER_ID} /bin/bash
