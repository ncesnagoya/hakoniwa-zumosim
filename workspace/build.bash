#!/bin/bash

echo "HELLO WORLD $#"
pwd

ls

if [ ! -d cmake-build ]
then
    mkdir cmake-build
fi

cd cmake-build
cmake ..
make

