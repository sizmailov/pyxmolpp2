Frame - Molecule - Residue - Atom
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


The ``pyxmolpp2`` library implements |Frame|/|Molecule|/|Residue|/|Atom| hierarchy to represent a molecular system.

Unlike many other molecular libraries |Atom|, |Residue| or |Molecule| do not exist on their own, they are always part of |Frame|.
Therefore any |Atom|, |Residue| or |Molecule| are guaranteed to have a parent.
This makes expression :py:`atom.residue.chain.frame.index` always valid and eliminates :py:`is not None` from user and
library code.

Of course, this makes construction / manipulating of incomplete structures a bit more complex,
but it's a conscious design choice since ``pyxmolpp2`` is targeted to MD processing.

Let's read pdb file and get first frame (i.e. ``MODEL`` in pdb terms):

.. py-exec::
    :context-id: FRCA


    import os
    from pyxmolpp2 import PdbFile

    filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")

    # read first model from pdb
    frame = PdbFile(filename).frames()[0]


Let's print info about first C-alpha atom:

.. py-exec::
    :context-id: FRCA

    a = frame["A"][1]["CA"]
    print(a.id, a.name, a.residue.name, a.molecule.name, a.r, sep='\n')


.. block-danger::
    Keep frame alive

    |Atom|/|Residue|/|Molecule| are alive until corresponding |Frame| exists.
    |Frame| exists until python holds a reference to it. No frame - no game.


Let's intentionally destroy frame, by dropping the only reference and see what
happens when we try to access stored atom reference:

.. py-exec::
    :context-id: FRCA
    :discard-context:
    :raises: RuntimeError

    frame = None
    a.name