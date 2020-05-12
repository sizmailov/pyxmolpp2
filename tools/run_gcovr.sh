#!/bin/bash
set -e 
make -C cmake-build-coverage all -j 4
#make -C cmake-build-coverage test

export PYTHONPATH=cmake-build-coverage
python -m pytest pytests

mkdir -p coverage 
gcovr cmake-build-coverage --root=./ --delete --exclude='external/.*' --exclude='tests/.*' --html --html-details  -o coverage/xmol.html
