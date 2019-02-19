#!/bin/bash

python -m pip install -r docs/requirements.txt
export TEST_DATA_PATH=$PWD/tests_dataset
make -C ./docs html
touch ./docs/build/html/.nojekyll
