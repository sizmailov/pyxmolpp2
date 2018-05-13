#!/bin/bash

function doCompile {
  pip install -r docs/requirements.txt
  sphinx-versioning build \
      docs/source docs/build/html \
      --run-setup-py \
      --whitelist-branches 'master|dev' \
      --whitelist-tags '.*' \
      --show-banner \
      --greatest-tag \
      --banner-greatest-tag
}
