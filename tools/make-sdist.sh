#!/bin/bash

pip install .
pip install black isort pybind11-stubgen
pybind11-stubgen pyxmolpp2._core -o ./ --numpy-array-wrap-with-annotated --exit-code
black ./pyxmolpp2
isort --profile=black ./pyxmolpp2
python setup.py sdist