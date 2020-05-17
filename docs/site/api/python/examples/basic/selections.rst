Selections basics
^^^^^^^^^^^^^^^^^

*Selection* is ordered set of elements in ``pyxmolpp2``. Order is defined as follows

1. if two elements belongs to same parent object, the order match their construction order
2. otherwise, they ordered as their parents
3. |Frame| references are ordered by :ref:`Frame.index`

.. py-exec::
    :context-id: selections
    :hide-code:

    import pyxmolpp2
    import os

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
    pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

    frame = pdb_file.get_frame()


Library has three types of `selections`: |AtomSelection|, |ResidueSelection| and |ChainSelection|

Any selections can be created from |Frame|:

.. py-exec::
    :context-id: selections

    print(frame.asChains)
    print(frame.asResidues)
    print(frame.asAtoms)


|AtomSelection| and |ResidueSelection| can be created from |Chain|:

.. py-exec::
    :context-id: selections

    chain = frame.asChains[0]
    print(chain.asResidues)
    print(chain.asAtoms)


|AtomSelection| can be created from a |Residue|:

.. py-exec::
    :context-id: selections

    residue = frame.asResidues[0]
    print(residue.asAtoms)

Selection can be converted up and down through hierarchy:

.. py-exec::
    :context-id: selections

    # selects non-empty residues
    print(chain.asAtoms.asResidues)
    # selects chains with at least 1 residue
    print(frame.asResidues.asChains)
    # select chains with at least 1 non-empty residue
    print(frame.asChains.asResidues.asAtoms.asResidues.asChains)

Filter selections
^^^^^^^^^^^^^^^^^

Selection ``.filter`` method returns new selection with elements that match given predicate:

.. py-exec::
    :context-id: selections

    from pyxmolpp2.polymer import AtomName, ResidueName

    chain.asAtoms.filter(lambda a: a.r.x < 0)  # select atoms with negative x coord
    chain.asAtoms.filter(lambda a: a.name == AtomName("CA"))  # select CA atoms
    chain.asResidues.filter(lambda r: r.name == ResidueName("LYS"))  # select LYS residues



``pyxmolpp2`` has pre-defined predicate generators for common filtering operations:

.. py-exec::
    :context-id: selections

    from pyxmolpp2.polymer import aName, rName, aId, rId, cName, cIndex

    frame.asAtoms.filter(aName == "CA")  # select CA atoms
    frame.asResidues.filter(rName == "LYS")  # select LYS residues
    frame.asChains.filter(cName == "A")  # select chain(s) A



|ChainPredicate| and |ResiduePredicate| can be naturally applied to |AtomSelection|,
while |AtomPredicate| can be applied only to |AtomSelection|.

.. py-exec::
    :context-id: selections

    frame.asAtoms.filter(aName == "CA")  # select CA atoms
    frame.asAtoms.filter(rName == "LYS")  # select all atoms of LYS residues


Application of |AtomPredicate| to |ResidueSelection| or |ChainSelection| leads to :ref:`TypeError` exception.
Same stands for |ResiduePredicate| and |ChainSelection|:

.. py-exec::
    :context-id: selections
    :raises: TypeError
    :hide-stderr:

    frame.asResidues.filter(aName == "CA")

.. py-exec::
    :context-id: selections
    :raises: TypeError
    :hide-stderr:

    frame.asChains.filter(aName == "CA")

.. py-exec::
    :context-id: selections
    :raises: TypeError
    :hide-stderr:

    frame.asChains.filter(rName == "LYS")


Predicates can be combined using ``&``, ``|``, ``~`` operators and reused:

.. py-exec::
    :context-id: selections

    from pyxmolpp2.polymer import AtomPredicate

    criteria = (aName == "CA") & rId.is_in({1, 2, 3, 4}) & AtomPredicate(lambda a: a.r.x < 0)  # type:AtomPredicate

    print(frame.asAtoms.filter(criteria | cName.is_in({"X", "Y", "Z"})))

Selection operations
^^^^^^^^^^^^^^^^^^^^

Selections support set operations:

- `union` (operators ``+``, ``+=``)
- `set intersection` (operators ``*``, ``*=``)
- `difference` (operators ``-``, ``-=``)


.. py-exec::
    :context-id: selections

    A = frame.asAtoms.filter(lambda a: a.r.x > 5)
    B = frame.asAtoms.filter(lambda a: a.r.x <= 5)

    print(A)
    print(B)
    print(A+B)
    print(A-B)
    print(A*B)

Selection invalidation
^^^^^^^^^^^^^^^^^^^^^^

In execution of the program selection may be marked as `invalid`, i.e. further access
to it's elements raises an exception.

Selections are invalidated on:

- destruction of any of it's elements parent (most likley deletion of a whole |Frame|.

.. note-info::

    The exception will be raised so you will know that you are doing something wrong.

- on :ref:`Frame.index` change if selection had elements from two frames or more.

.. note-danger::

   There is no runtime checks against this type of errors.

