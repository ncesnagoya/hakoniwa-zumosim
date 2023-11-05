#!/bin/bash

cd $(dirname $0)

if [ ! -d cmake-build ]
then
    mkdir cmake-build
fi

cd cmake-build

if [ $# -eq 1 ]
then
    HAKO_CPP_SOURCE_DIR=${1}
    if [ ! -d ${HAKO_CPP_SOURCE_DIR} ]
    then
        echo "ERROR: can not find hakoniwa-core-cpp-client dir: ${HAKO_CPP_SOURCE_DIR}"
        exit 1
    fi
    if [ ! -f /usr/local/lib/hakoniwa//libhako.a ]
    then
        sudo cp ${HAKO_CPP_SOURCE_DIR}/cmake-build/core/src/libhako.a /usr/local/lib/hakoniwa/
    fi
    cmake .. -DHAKO_CPP_SOURCE_DIR=${HAKO_CPP_SOURCE_DIR}
else
    cmake ..
fi

make
