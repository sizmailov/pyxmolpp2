Selections
^^^^^^^^^^


*Selection* is ordered set of elements in ``pyxmolpp2``. Order is defined as follows

1. if two children belong to same parent object, the order match children construction order
2. otherwise, they ordered as their parents


.. py-exec::
    :context-id: selections
    :hide-code:

    import pyxmolpp2
    import os

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
    pdb_file = pyxmolpp2.PdbFile(pdb_filename)

    frame = pdb_file.frames()[0]

.. block-info:: Span vs Selections

    There are two slightly different types of "selections" in the library.
    A ``Span`` is a continuous selections of elements, while ``Selection`` is an arbitrary set of elements.
    Those two forms of selections functionally almost the same so you won't notice any difference most of the time.

    In what follows both are referred as selections.

    .. py-exec::
        :context-id: selections

        print(frame.atoms[10:20])   # AtomSpan
        print(frame.atoms[10:20:2]) # AtomSelection



Library has three types of `selections`: |AtomSelection|, |ResidueSelection| and |MoleculeSelection|

Selections can be created in a number of ways:

.. py-exec::
    :context-id: selections

    # from frame
    print(frame.molecules)
    print(frame.residues)
    print(frame.atoms)

.. py-exec::
    :context-id: selections

    # Construct from another selection
    print(frame.molecules.atoms)
    print(frame.residues.molecules)
    print(frame.atoms.residues)

.. py-exec::
    :context-id: selections

    # From an iterable
    from pyxmolpp2 import AtomSelection
    print(AtomSelection((atom for atom in frame.residues[:2].atoms )))


Filter selections
^^^^^^^^^^^^^^^^^

Selection ``.filter`` method returns new selection with elements that match given predicate:

.. py-exec::
    :context-id: selections

    mol = frame.molecules[0]

    mol.atoms.filter(lambda a: a.r.x < 0)           # select atoms with negative x coord
    mol.atoms.filter(lambda a: a.name == "CA")      # select CA atoms
    mol.residues.filter(lambda r: r.name == "LYS")  # select LYS residues



``pyxmolpp2`` has pre-defined predicate generators for common filtering operations:

.. py-exec::
    :context-id: selections

    from pyxmolpp2 import aName, rName, aId, rId, mName

    frame.atoms.filter(aName == "CA")      # select CA atoms
    frame.residues.filter(rName == "LYS")  # select LYS residues
    frame.molecules.filter(mName == "A")   # select molecule(s) with name A



|MoleculePredicate| and |ResiduePredicate| can be naturally applied to |AtomSelection|,
while |AtomPredicate| can be applied only to |AtomSelection|.

.. py-exec::
    :context-id: selections

    frame.atoms.filter(aName == "CA")  # select CA atoms
    frame.atoms.filter(rName == "LYS")  # select all atoms of LYS residues


Application of |AtomPredicate| to |ResidueSelection| or |MoleculeSelection| leads to :ref:`TypeError` exception.
Same stands for |ResiduePredicate| and |MoleculeSelection|:

.. py-exec::
    :context-id: selections
    :raises: TypeError
    :hide-stderr:

    frame.residues.filter(aName == "CA")

.. py-exec::
    :context-id: selections
    :raises: TypeError
    :hide-stderr:

    frame.molecules.filter(aName == "CA")

.. py-exec::
    :context-id: selections
    :raises: TypeError
    :hide-stderr:

    frame.molecules.filter(rName == "LYS")


Predicates can be combined using ``&``, ``|``, ``~`` operators and reused:

.. py-exec::
    :context-id: selections

    from pyxmolpp2 import AtomPredicate

    criteria = (aName == "CA") & rId.is_in({1, 2, 3, 4}) & AtomPredicate(lambda a: a.r.x < 0)
    print(criteria)
    print(frame.atoms.filter(criteria | mName.is_in({"X", "Y", "Z"})))

Selection operations
^^^^^^^^^^^^^^^^^^^^

Selections support set operations:

- `union`, operator ``|``
- `set intersection`, operator ``&``
- `difference`, operator ``-``


.. py-exec::
    :context-id: selections

    A = frame.atoms.filter(lambda a: a.r.x > 5)
    B = frame.atoms.filter(lambda a: a.r.x <= 5)

    print(A)
    print(B)
    print(A | B)
    print(A - B)
    print(A & B)

Selections invalidation
^^^^^^^^^^^^^^^^^^^^^^^

In execution of the program selection or element may be marked as `invalid`, i.e. further access
to it's elements raises an exception.

Selections are invalidated on:

- destruction of any of it's elements parent (most likley deletion of a whole |Frame|.

.. note-info::

    The exception will be raised so you will know that you are doing something wrong.

