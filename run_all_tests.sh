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
  if [ "${build_dir}" = "cmake-build-release" ] ; then 
     export PYXMOLPP_RUN_PERFORMANCE_TESTS=1
  else 
     unset PYXMOLPP_RUN_PERFORMANCE_TESTS
  fi
  pytest -s pytests
done 

