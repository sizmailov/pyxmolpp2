#!/bin/bash

for build_dir in cmake-build-*
do 
make -C ${build_dir} all 
make -C ${build_dir} test 
done 

