#!/bin/bash

echo $#

if [ $# -eq 0 ]; then 
	echo "construct..."
	cmake -DENABLE_CREATE_TEST=ON ..
	make -j4
	make install
	exit 0
fi


if [ $# -eq 1 ]; then
    if [ $1 = "clean" ]; then
		echo "start remove tmp files."
		rm -rf CMakeCache.txt CMakeFiles
		rm -rf cmake_install.cmake  test-scppl scppl install_manifest.txt Makefile
		exit 0
	fi 

    if [ $1 = "lib" ]; then
		echo "start compile test exe."
		cmake -DENABLE_CREATE_TEST=OFF ..
		make clean
		make -j4
		make  install
		echo ""
		echo -e "\033[31m Please enter bin dir to execute. \033[0m"
		echo ""
		exit 0
    fi 
fi