#!/bin/bash
set -e
( cd _build && make -j 4 all && ctest ) ;
gcovr _build --root=./ --delete --filter='src/.*' --filter='include/.*' --filter='pyxmolpp/.*'  --json -o cpp-tests.json ;
( export PYTHONPATH=$PWD/_build && python -m pytest tests/bindings ) ;
gcovr _build --root=./ --delete --filter='src/.*' --filter='include/.*' --filter='pyxmolpp/.*'  --json -o py-tests.json ;
gcovr  --add-tracefile py-tests.json --add-tracefile cpp-tests.json --xml -o coverage.xml ;

if [ -f .cc_token ]; then
  bash <(curl -s https://codecov.io/bash) -f coverage.xml -t @.cc_token
else
  # on CI
  bash <(curl -s https://codecov.io/bash) -f coverage.xml
fi