#!/bin/bash

if [ -d build ];
then
	cmake -S . -B build
	cmake --build build -j$(nproc)
else
    mkdir build
    cmake -S . -B build
    cmake --build build -j$(nproc)
fi
