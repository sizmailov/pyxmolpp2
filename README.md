### A tool to process MD trajectories

[![Build Status](https://travis-ci.org/sizmailov/pyxmolpp2.svg?branch=master)](https://travis-ci.org/sizmailov/pyxmolpp2) [![codecov](https://codecov.io/gh/sizmailov/pyxmolpp2/branch/master/graph/badge.svg)](https://codecov.io/gh/sizmailov/pyxmolpp2)


## Goal 

We want our scripts to 
 - run fast
 - be easy to read  
 - be easy to write correct
 - be hard to write incorrect

So we need a library which 
 - is reliable
 - full of features


Well-written C++ code gives us speed. An IDE augmented with static type system should prevent many occasional 
pythonic mistakes before script is run. Moderate functional style programming contributes to readability.
At least it's how it was meant to be)


## Feature list

To be written

## Usage examples

```python

from pyxmolpp2.pdb import PdbFile
from pyxmolpp2.geometry.functional import translate_by


frame = PdbFile("test.pdb").get_frame()

# place geometric center at (0,0,0)
frame.asAtoms.for_each(translate_by(XYZ(1,2,3)))

# write as pdb 
frame.to_pdb("out.pdb")

```

## Install

To install from master branch run command
```bash
python -m pip install git+git@github.com:sizmailov/pyxmolpp2.git
```

## Reference

To be written

## Project structure

1. C++ library `xmol`, the core
2. Python library `pyxmolpp2`, wraps `xmol` by `pybind11`
3. User scripts, are written for daily duties  



## Credits

Written by Sergei Izmailov in BioNMR lab at SPbSU
