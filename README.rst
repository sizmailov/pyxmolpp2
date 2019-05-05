#################################
A tool to process MD trajectories
#################################

.. |master_build| image:: https://travis-ci.com/sizmailov/pyxmolpp2.svg?branch=master
    :alt: master status
    :target: https://travis-ci.com/sizmailov/pyxmolpp2


.. |master_coverage| image:: https://codecov.io/gh/sizmailov/pyxmolpp2/branch/master/graph/badge.svg
    :alt: master coverage
    :target: https://codecov.io/gh/sizmailov/pyxmolpp2/branch/master


.. |dev_build| image:: https://travis-ci.com/sizmailov/pyxmolpp2.svg?branch=dev
    :alt: dev status
    :target: https://travis-ci.com/sizmailov/pyxmolpp2


.. |dev_coverage| image:: https://codecov.io/gh/sizmailov/pyxmolpp2/branch/dev/graph/badge.svg
    :alt: dev coverage
    :target: https://codecov.io/gh/sizmailov/pyxmolpp2/branch/dev

.. image:: https://badge.fury.io/py/pyxmolpp2.svg
    :target: https://badge.fury.io/py/pyxmolpp2

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

-------------
Documentation
-------------

https://sizmailov.github.io/pyxmolpp2/

It includes tutorial, usage examples and python API reference.

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

**Caution**: Total size of repository with sub-repositories exceeds 100mb

To install from master branch run command

.. code-block:: bash

    python -m pip install git+https://github.com/sizmailov/pyxmolpp2.git


-------
Credits
-------

Written by Sergei Izmailov in BioNMR lab at SPbSU
