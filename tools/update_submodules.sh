#!/bin/bash

set -e

wd=$PWD


cd "external/eigen-git-mirror"
git remote | grep upstream || git remote add upstream https://github.com/eigenteam/eigen-git-mirror.git
git checkout master
git pull upstream master
cd "$wd"


cd "external/googletest"
git remote | grep upstream || git remote add upstream https://github.com/google/googletest.git
git checkout master
git pull upstream master
cd "$wd"

cd "external/GSL"
git remote | grep upstream || git remote add upstream https://github.com/Microsoft/GSL.git
git checkout master
git pull upstream master
cd "$wd"

cd "external/pybind11"
git remote | grep upstream || git remote add upstream https://github.com/pybind/pybind11.git
git checkout master
git pull upstream master
cd "$wd"


cd "external/range-v3"
git remote | grep upstream || git remote add upstream https://github.com/ericniebler/range-v3.git
git checkout master
git pull upstream master
cd "$wd"

bash run_all_tests.sh


cd "external/eigen-git-mirror"
git push origin master
cd "$wd"


cd "external/googletest"
git push origin master
cd "$wd"

cd "external/GSL"
git push origin master
cd "$wd"

cd "external/pybind11"
git push origin master
cd "$wd"


cd "external/range-v3"
git push origin master
cd "$wd"
