#!/bin/bash

set -e

for build_dir in cmake-build-*
do 
  make -j 4 -C ${build_dir} all
done


if [[ ! -d tests_dataset ]]; then
  git clone git@github.com:sizmailov/pyxmopp_tests_dataset.git tests_dataset
fi

for build_dir in cmake-build-*
do
  make -C ${build_dir} test
  export PYTHONPATH=${build_dir}
  if [ "${build_dir}" = "cmake-build-release" ] ; then 
     export PYXMOLPP_RUN_PERFORMANCE_TESTS=1
  else 
     unset PYXMOLPP_RUN_PERFORMANCE_TESTS
  fi
  python -m pytest -s pytests
done 

