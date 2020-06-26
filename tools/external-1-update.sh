#!/bin/bash

set -x

WD=$PWD

cd "$WD/external/eigen-git-mirror"
git checkout master
git pull upstream master

cd "$WD/external/googletest"
git checkout master
git pull upstream master

cd "$WD/external/benchmark"
git checkout master
git pull upstream master

cd "$WD/external/GSL"
git checkout master
git pull upstream master

cd "$WD/external/pybind11"
git checkout master
git pull upstream master
