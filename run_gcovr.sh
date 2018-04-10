#!/bin/bash

gcovr --root=./ --exclude='external/.*' --exclude='tests/.*' --html --html-details  -o coverage/xmol.html
