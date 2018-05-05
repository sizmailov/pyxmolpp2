
.. |Atom| replace:: :py:class:`~pyxmolpp2.polymer.Atom`
.. |Residue| replace:: :py:class:`~pyxmolpp2.polymer.Residue`
.. |Chain| replace:: :py:class:`~pyxmolpp2.polymer.Chain`
.. |Frame| replace:: :py:class:`~pyxmolpp2.polymer.Frame`

.. |AtomSelection| replace:: :py:class:`~pyxmolpp2.polymer.AtomSelection`
.. |ResidueSelection| replace:: :py:class:`~pyxmolpp2.polymer.ResidueSelection`
.. |ChainSelection| replace:: :py:class:`~pyxmolpp2.polymer.ChainSelection`



First steps
===========


Reading pdb file
----------------

If your `.pdb` file conforms `Atomic Coordinate Entry Format Version 3.3 <http://www.wwpdb.org/documentation/file-format-content/format33/v3.3.html>`_
you can read it a via :py:meth:`pyxmolpp2.pdb.PdbFile`:

.. code-block:: python

    from pyxmolpp2.pdb import PdbFile
    frame = PdbFile("sample.pdb").get_frame()
    print(frame.size)


Reading non-standard pdb file
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you want to read non-standard pdb file you should provide altered pdb records that match your file.
For example one may want to adjust ``serial`` field of ``ATOM`` record:


.. code-block:: python

    from pyxmolpp2.pdb import *
    altered_records = AlteredPdbRecords(StandardPdbRecords.instance)
    altered_records.alter_record(RecordName("ATOM"),FieldName("serial"),[7,12])
    frame = PdbFile("sample.pdb").get_frame(altered_records)
    print(frame.size)

See :py:class:`pyxmolpp2.pdb.AlteredPdbRecords`

.. caution::
    Reading of pdb files with variable position of fields is not supported:

    ::

      ATOM     32  N  AARG A  -3      11.281  86.699  94.383  0.50 35.88           N
      ATOM       33  CA AARG A  -3      12.353  85.696  94.456  0.50 36.67           C


Selections
----------

Library has three types of `selections`: |AtomSelection|, |ResidueSelection|, |ChainSelection| - ordered sets of elements.

Construction
^^^^^^^^^^^^

Any selections might be created from |Frame| instance,   |AtomSelection| and |ResidueSelection| can be created
from |Chain| instance, |AtomSelection| could be created from a |Residue|::

    print(frame.asAtoms.size) # number of atoms in frame
    print(frame.asResidues.size) # number of residues in frame
    print(frame.asChains.size)# number of chains in frame

    chain = frame.asChains[0]
    print(chain.asAtoms.size) # number of atoms in first chain
    print(chain.asResidues.size) # number of residues in first chain

    residue = frame.asResidues[-1]
    print(residue.asAtoms.size) # number of atoms in frame last residue


Conversions
^^^^^^^^^^^

Selections might be converted up and down thought hierarchy::

    chain.asAtoms.asResidues # selects non-empty residues
    frame.asResidues.asChains # selects chains with at least 1 residue
    frame.asChains.asResidues.asAtoms.asResidues.asChains # selects chains with at least 1 non-empty residue

Filter
^^^^^^
A selection could be filtered inplace via `filter` method::

    chain.asAtoms.filter(lambda a: a.r.x < 0) # select atoms with negative x coord
    chain.asAtoms.filter(lambda a: a.name == AtomName("CA")) # select CA atoms
    chain.asResidues.filter(lambda r: r.name == ResidueName("LYS")) # select LYS residues


Set operations
^^^^^^^^^^^^^^

Selections support number set operations:
    - `union` (operators ``+``, ``+=``)
    - `set intersection` (operators ``*``, ``*=``)
    - `difference` (operators ``-``, ``-=``)


.. code-block:: python

    A = frame.asAtoms.filter(lambda a: a.x > 0)
    B = frame.asAtoms.filter(lambda a: a.x <= 0)

    C = A+B
    C = A-B
    C += B

    D = A*B


Invalidation of selection
^^^^^^^^^^^^^^^^^^^^^^^^^

In execution of the program selection may be marked as `invalid`, i.e. further access
to it's elements raises an exception.

Selection gets invalidated on:
    - destruction of any of it's elements parent (Primary this happens on deletion of a whole |Frame|)
    - on :py:attr:`~pyxmolpp2.polymer.Frame.index` change if selection had elements from two frames or more

Atom/Residue/Chain references
=============================

It's allowed to store references to Atom/Residue/Chain/Frame in python code. They are
guaranteed to be not-None, while they might be invalidated if corresponding structure was destroyed.

Access to invalid reference results to exception.


Strict hierarchy rationale
==========================

The `pyxmolpp2` library implements |Frame|/|Chain|/|Residue|/|Atom| hierarchy to represent a molecular system.

Every |Atom| exists as a part of some |Residue|. |Residue| is always a part of |Chain|. |Chain| is always a part of |Frame|.

For instance this implies that you can not create an |Atom| without pre-existed |Residue|.
Why not allow ``Atom.residue`` to be :py:class:`None` if Atom exists by it's own?
The choice was between flexibility of construction code versus complexity of it's further usage.
The rationale behind that design decision is fact that expression ``atom.residue.chain.frame.index``
is always correct, and no `not-None` checks are required. Such checks across the library and user code would
increases it's complexity and make it more prone to errors.


Keep your frame alive
=====================

|Atom| is alive until it's |Frame| exists. |Frame| exists until python holds a reference to it.
No frame - no game.
