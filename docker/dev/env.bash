#!/bin/bash

export HOST_WORKDIR=`pwd`
export DOCKER_WORKDIR=`pwd`


export HOST_ZUMO_INFDIR=`pwd`/hakoniwa-zumoIf/include
export DOCKER_ZUMO_INFDIR=/root/hakoniwa-zumoIf/include

export DOCKER_RUN_CMD=${DOCKER_WORKDIR}/workspace/build.bash