#!/bin/bash

cd ./src
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=RelWithDbgInfo -B ../build 
cd ../build
ninja 