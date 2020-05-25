Reorder atoms/residues
^^^^^^^^^^^^^^^^^^^^^^

In this example we will see how to reorder atoms/residues/chains

.. py-exec::
    :context-id: reorder_atoms

    import os
    import pyxmolpp2

Let's create a frame to work with

.. py-exec::
    :context-id: reorder_atoms

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
    pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

.. py-exec::
    :context-id: reorder_atoms

    frame = pdb_file.get_frame()

    print([a.id for a in frame.asAtoms])

First we need to define new order of atoms.
For sake of simplicity let's number them in reverse order

.. py-exec::
    :context-id: reorder_atoms

    atoms = frame.asAtoms

    for i, a in enumerate(atoms):
        a.id = atoms.size - i

    # New atom ids:
    print([a.id for a in frame.asAtoms])


As you can see :ref:`id <Atom.id>` does not affect atom order in selections.

We can create a new Frame with altered atom order `within` residue.

.. py-exec::
    :context-id: reorder_atoms
    :discard-context:

    from pyxmolpp2.polymer import Frame

    # create empty frame with index=0
    new_frame = Frame(0)

    for chain in frame.asChains:
        # create empty chain with same name
        new_chain = new_frame.emplace(chain.name)
        for residue in frame.asResidues:
            # create empty residue with same name and id
            new_residue = new_chain.emplace(residue.name, residue.id)
            for a in sorted(list(residue.asAtoms), key=lambda a: a.id):
                new_atom = new_residue.emplace(a)  # create a copy of atom `a`

    # New frame atoms ids:
    print([a.id for a in new_frame.asAtoms])

.. block-info::
    Hierarchy defines order

    Note that frame order is superior to chain order, chain order is superior to residue order,
    residue order is superior to atom order. In other words children order follows parents order.
