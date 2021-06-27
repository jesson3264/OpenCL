#!/bin/bash

cd ./datastruct/build/
cmake ..
cd -

cd ./memory/build/
cmake ..
cd -
#
pwd
cd build
ar x ../memory/build/libmemory.a 

ar x ../datastruct/build/libdatastruct.a 

ar rc libscl.a *.o

cd -

#ar rcs libscl.a ./memeory/build/libmemory.a ./datastruct/build/libdatastruct.a



