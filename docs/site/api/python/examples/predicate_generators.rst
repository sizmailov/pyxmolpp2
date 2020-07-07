Predicate generators
####################

:ref-prefix:
    pyxmolpp2

In this example we will see how selections of atoms/residues/molecules can be created using predicates.

We start from importing required stuff from :ref:`pyxmolpp2` and reading the :py:`frame`

.. py-exec::
    :context-id: selections

    from pyxmolpp2 import PdbFile
    import os

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
    frame = PdbFile(pdb_filename).frames()[0]

Let's print structural info about our frame:

.. py-exec::
    :context-id: selections

    print('\n'.join([
        f"Molecule {mol.name:1s}: ({mol.residues.size} residues)\n    " +
        ", ".join([f"{r.name}-{r.id}" for r in mol.residues])
        for mol in frame.molecules
    ]))

.. note-dim::
    Molecules vs Chains

    Here all water molecules were put on second molecule. The PDB reader expects different molecules
    to be separated by ``TER`` records

Let's filter frame residues with some arbitrary predicates:

.. py-exec::
    :context-id: selections

    def adhoc_predicate(r):
        return (r.id <= 10) and (r.id not in {2, 4, 9}) and (r.name not in {"GLY", "PRO"})

    print(frame.residues.filter(lambda r: r.name == "GLY"))
    print(frame.residues.filter(lambda r: r.id <= 10))
    print(frame.residues.filter(adhoc_predicate))

Using lambda or ad-hoc predicate functions adds a bit of boilerplate, which can be avoided by using predicate generators:
:ref:`aName`, :ref:`aId`, :ref:`rId`, :ref:`rName`, :ref:`mName`.
Predicate generators provide a convenient syntax to construct & combine predicates:

.. py-exec::
    :context-id: selections

    from pyxmolpp2 import aName, aId, rName, rId, mName

    print(frame.residues.filter(rName == "GLY"))
    print(frame.residues.filter(rId <= 10))
    print(frame.residues.filter((rId <= 10) & ~rId.is_in(2,4,9) & ~rName.is_in("GLY", "PRO")))

An extra bonus generated :ref:`ResiduePredicate` can be naturally applied to atoms as well:

.. py-exec::
    :context-id: selections
    :discard-context:

    print(frame.atoms.filter(rName == "GLY"))
    print(frame.atoms.filter(rId <= 10))
    print(frame.atoms.filter( (rId <= 10) & ~rId.is_in(2,4,9) & ~rName.is_in("GLY", "PRO")))
