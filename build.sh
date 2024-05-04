#!/bin/bash

cmake -G "Ninja" -DSPECTRAL_LIB_ONLY=0 -DSPECTRAL_DISABLE_PROGRESS_BAR=0 -DCMAKE_BUILD_TYPE=RelWithDbgInfo -B ./build 
cd ./build
ninja 