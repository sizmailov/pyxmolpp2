#################################
A tool to process MD trajectories
#################################

.. |master_build| image:: https://travis-ci.org/sizmailov/pyxmolpp2.svg?branch=master
    :alt: master status
    :target: https://travis-ci.org/sizmailov/pyxmolpp2


.. |master_coverage| image:: https://codecov.io/gh/sizmailov/pyxmolpp2/branch/master/graph/badge.svg
    :alt: master coverage
    :target: https://codecov.io/gh/sizmailov/pyxmolpp2/branch/master


.. |dev_build| image:: https://travis-ci.org/sizmailov/pyxmolpp2.svg?branch=dev
    :alt: dev status
    :target: https://travis-ci.org/sizmailov/pyxmolpp2


.. |dev_coverage| image:: https://codecov.io/gh/sizmailov/pyxmolpp2/branch/dev/graph/badge.svg
    :alt: dev coverage
    :target: https://travis-ci.org/sizmailov/pyxmolpp2/branch/dev


================= =================
    **master**         **dev**
================= =================
|master_build|    |dev_build|
|master_coverage| |dev_coverage|
================= =================

----
Goal
----

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

------------
Feature list
------------

To be written

--------------
Usage examples
--------------

Translate all atoms by (1,2,3):

.. code-block:: python

    from pyxmolpp2.pdb import PdbFile
    from pyxmolpp2.geometry import XYZ

    def translate_by(dr):
        def translate(a):
            a.r = a.r+dr
        return translate

    frame = PdbFile("test.pdb").get_frame()

    frame.asAtoms.for_each(translate_by(XYZ(1,2,3)))

    # write as pdb
    frame.to_pdb("out.pdb")



-------
Install
-------

from PyPi package
^^^^^^^^^^^^^^^^^

PyPi package contains all C++ dependencies, total size is about ~4 mb.
Install command:

.. code-block:: bash

    python -m pip install pyxmolpp2



from git repository
^^^^^^^^^^^^^^^^^^^
.. caution:: Total size of repository with sub-repositories exceeds 100mb

To install from master branch run command

.. code-block:: bash

    python -m pip install git+https://github.com/sizmailov/pyxmolpp2.git



-------------
API Reference
-------------

To be written

-----------------
Project structure
-----------------

1. C++ library ``xmol``, the core
2. Python library ``pyxmolpp2``, wraps ``xmol`` by ``pybind11``
3. User scripts, are written for daily duties  


-------
Credits
-------

Written by Sergei Izmailov in BioNMR lab at SPbSU
