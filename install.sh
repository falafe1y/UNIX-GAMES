#!/bin/bash

if [ -d build ];
then
	cd build
	cmake --build .
else
    mkdir build
    cd build
    cmake ..
    cmake --build .
fi

./FGames
