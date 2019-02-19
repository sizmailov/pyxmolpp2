
Installation
------------

from PyPi package
^^^^^^^^^^^^^^^^^

.. warning::

    If your default C++ compiler does not have full C++11 support you will see messy error
    message during installation and you must set environment variables to point to modern
    version of C++ compiler (g++>=5.0 or clang>=3.6), for example:

    .. code-block:: bash

        export CC=gcc-7.3.0 ; export CXX=g++-7.3.0 ;



PyPi package contains all C++ dependencies, total size is about ~4 mb.
Install command:

.. code-block:: bash

    python -m pip install pyxmolpp2



from git repository
^^^^^^^^^^^^^^^^^^^

.. note::

    Total size of repository with sub-repositories exceeds 100mb

To install from master branch run command

.. code-block:: bash

    python -m pip install git+https://github.com/sizmailov/pyxmolpp2.git


Generation stubs for PyCharm/mypy
---------------------------------

``pyxmolpp2`` is a binary python module, so it can't be readily understood by static analysis tools.
The ``pyxmolpp2-stubs`` package generates stubs for installed ``pyxmolpp2`` module.


.. code-block:: bash

    # install from PyPI
    python -m pip install pyxmolpp2-stubs

    # install from github
    python -m pip install git+https://github.com/sizmailov/pyxmolpp2-stubs.git

