#!/usr/bin/bash

mkdir ./cmake-build-debug;
cd ./cmake-build-debug || exit;
mkdir -p tmp;
cmake -G"Unix Makefiles" ..;
make;
