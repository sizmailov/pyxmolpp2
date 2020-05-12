#!/bin/bash

WD=$PWD

cd "$WD/external/eigen-git-mirror"
git remote add upstream git@github.com:eigenteam/eigen-git-mirror.git

cd "$WD/external/googletest"
git remote add upstream git@github.com:google/googletest.git

cd "$WD/external/GSL"
git remote add upstream git@github.com:microsoft/GSL.git

cd "$WD/external/pybind11"
git remote add upstream git@github.com:pybind/pybind11.git

cd "$WD/external/range-v3"
git remote add upstream git@github.com:ericniebler/range-v3.git