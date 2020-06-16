#!/bin/bash
set -e
( cd _build && ctest ) ;
gcovr _build --root=./ --delete --filter='src/.*' --filter='include/.*' --filter='pyxmolpp/.*'  --json -o cpp-tests.json ;
( export PYTHONPATH=$PWD/_build && python -m pytest tests/bindings ) ;
gcovr _build --root=./ --delete --filter='src/.*' --filter='include/.*' --filter='pyxmolpp/.*'  --json -o py-tests.json ;
gcovr  --add-tracefile py-tests.json --add-tracefile cpp-tests.json --xml -o coverage.xml ;