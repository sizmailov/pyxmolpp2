
Installation
============

``pyxmolpp2`` is C++ extension and distributed as source files, so it might take a while to compile.


.. note-danger::

    Windows platform is not yet supported, see :gh:`sizmailov/pyxmolpp2#54`

Install dependencies
--------------------

``pyxmolpp2`` requires C++11 compiler, ``cmake`` and ``netcdf`` library to be installed on your system.

E.g. on modern ubuntu you can install requried packages via

.. code:: sh

    sudo apt-get cmake g++ python3 python3-dev libnetcdf-dev

.. note-info::

    If C++11-compatible compiler is not default one set env variables prior installation, e.g.:

    .. code-block:: sh

        export CC=gcc-7.3.0 ; export CXX=g++-7.3.0 ;

Install
-------

Package can be installed from PYPI (~4 MB):

.. code-block:: sh

    python -m pip install pyxmolpp2

