Frame - Chain - Residue - Atom
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


The ``pyxmolpp2`` library implements |Frame|/|Chain|/|Residue|/|Atom| hierarchy to represent a molecular system.

Unlike many other molecular libraries |Atom|, |Residue| or |Chain| do not exist on their own, they are always part of |Frame|.
Therefore any |Atom|, |Residue| or |Chain| are guaranteed to have a parent.
This makes expression :py:`atom.residue.chain.frame.index` always valid and eliminates :py:`is not None` from user and
library code.

Of course, this makes construction / manipulating of incomplete structures a bit more complex,
but it's a conscious design choice since ``pyxmolpp2`` is targeted to MD processing.

Let's read pdb file and get first frame (i.e. ``MODEL`` in pdb terms):

.. py-exec::
    :context-id: FRCA


    import os
    import pyxmolpp2

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
    pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

    frame = pdb_file.get_frame()


Let's print info about first C-alpha atom:

.. py-exec::
    :context-id: FRCA

    from pyxmolpp2.polymer import ChainName,AtomName

    a = frame[ChainName("A")][1][AtomName("CA")]
    print(a.id, a.name, a.residue.name, a.chain.name, a.r, sep='\n')



.. block-danger::
    Keep frame alive

    |Atom|/|Residue|/|Chain| are alive until corresponding |Frame| exists.
    |Frame| exists until python holds a reference to it. No frame - no game.


Let's intentionally destroy frame, by dropping the only reference and see what
happens when we try to access stored atom reference:

.. py-exec::
    :context-id: FRCA
    :discard-context:
    :raises: RuntimeError

    frame = None
    a.name