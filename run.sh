#!/usr/bin/bash

cmake --build build

if [ $# -ne 1 ]; then
    ./build/qz 
    exit 1
else 
    ./build/qz "$1"
    exit 0
fi