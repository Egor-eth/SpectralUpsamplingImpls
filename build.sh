#!/bin/bash

cd ./src
cmake -G "Ninja" -B ../build 
cd ../build
ninja