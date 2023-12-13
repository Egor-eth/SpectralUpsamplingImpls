#!/bin/bash

N_JOBS=${1:-4}

cd ./src
cmake -G "Ninja"  -B ../build 
cd ../build
ninja -j $N_JOBS