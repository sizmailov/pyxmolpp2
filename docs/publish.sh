#!/bin/bash

COMMIT_HASH=$(git rev-parse HEAD)

cd _/site
git init .
git checkout -b gh-pages
git add .
git commit -m "Deploy $COMMIT_HASH"
git remote add origin git@github.com:sizmailov/pyxmolpp2.git
git push origin gh-pages --force

