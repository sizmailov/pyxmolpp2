#!/bin/bash

set -e

for build_dir in cmake-build-*
do 
  make -j 4 -C ${build_dir} all
done

for build_dir in cmake-build-*
do
  make -C ${build_dir} test
  export PYTHONPATH=${build_dir}
  pytest pytests
done 

