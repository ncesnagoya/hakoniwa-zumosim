#!/bin/bash

source docker/run/env.bash

if [ $# -ne 0 ]
then
	USE_TTY=
else
	USE_TTY="t"
fi

HAKONIWA_TOP_DIR=`pwd`
IMAGE_NAME=`cat docker/run/image_name.txt`
IMAGE_TAG=`cat docker/run/latest_version.txt`
DOCKER_IMAGE=ncesnagoya/${IMAGE_NAME}:${IMAGE_TAG}


ARCH=`arch`
OS_TYPE=`bash docker/utils/detect_os_type.bash`
echo $ARCH
echo $OS_TYPE
if [ ${OS_TYPE} != "Mac" ]
then
	docker ps > /dev/null
	if [ $? -ne 0 ]
	then
	    sudo service docker start
	    echo "waiting for docker service activation.. "
	    sleep 3
	fi
fi

if [ ${OS_TYPE} = "wsl2" ]
then
	export RESOLV_IPADDR=`cat /etc/resolv.conf  | grep nameserver | awk '{print $NF}'`
	NETWORK_INTERFACE=$(route | grep '^default' | grep -o '[^ ]*$' | tr -d '\n')
	CORE_IPADDR=$(ip addr | grep inet | grep  "${NETWORK_INTERFACE}"  | awk '{print $2}' | awk -F/ '{print $1}')
elif [ ${OS_TYPE} = "Mac" ]
then
	CORE_IPADDR="127.0.0.1"
else
	IPADDR="127.0.0.1"
fi

if [ $ARCH = "arm64" ]
then
	docker run \
        -v ${HOST_WORKDIR}:${DOCKER_WORKDIR} \
        --platform linux/amd64 \
		-i${USE_TTY} --rm \
		--net host \
		-e CORE_IPADDR=${CORE_IPADDR} \
		-e DELTA_MSEC=${DELTA_MSEC} \
		-e MAX_DELAY_MSEC=${MAX_DELAY_MSEC} \
		-e GRPC_PORT=${GRPC_PORT} \
		-e UDP_SRV_PORT=${UDP_SRV_PORT} \
		-e UDP_SND_PORT=${UDP_SND_PORT} \
		--name ${IMAGE_NAME} ${DOCKER_IMAGE} 
else
	docker run \
        -v ${HOST_WORKDIR}:${DOCKER_WORKDIR} \
		-i${USE_TTY} --rm \
		--net host \
		-e CORE_IPADDR=${CORE_IPADDR} \
		-e DELTA_MSEC=${DELTA_MSEC} \
		-e MAX_DELAY_MSEC=${MAX_DELAY_MSEC} \
		-e GRPC_PORT=${GRPC_PORT} \
		-e UDP_SRV_PORT=${UDP_SRV_PORT} \
		-e UDP_SND_PORT=${UDP_SND_PORT} \
		--name ${IMAGE_NAME} ${DOCKER_IMAGE} 
fi
