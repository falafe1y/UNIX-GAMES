#!/bin/bash

if [ -d build ];
then
	echo "Deleting build directory..."
	rm -rf build
	echo "Directory build was deleted"	

	mkdir build
	cd build
	cmake ..
#	 make VERBOSE=1
	cmake --build .
else
    mkdir build
    cd build
    cmake ..
#    make VERBOSE=1
    cmake --build .
fi
