#!/bin/bash

make -C cmake-build-coverage all 
make -C cmake-build-coverage test 

gcovr --root=./ --exclude='external/.*' --exclude='tests/.*' --html --html-details  -o coverage/xmol.html
