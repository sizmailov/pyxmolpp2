#!/bin/bash

python -m pip install -r requirements.txt

wd="$PWD"
export TEST_DATA_PATH="$PWD/../tests_dataset"

if [[ ! -d m.css ]]; then
  # TODO: switch to official repo once PR#143, PR#145 accepted
  # git clone git://github.com/mosra/m.css
  git clone --depth 1  --branch all-fixes --single-branch --origin sizmailov git://github.com/sizmailov/m.css
fi

cd m.css/documentation

python python.py "$wd/site/api/python/conf.py"
python doxygen.py "$wd/../Doxyfile-mcss"
python python.py "$wd/site/conf.py"
