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
    pdb_file = pyxmolpp2.PdbFile(pdb_filename)

.. py-exec::
    :context-id: reorder_atoms

    frame = pdb_file.frames()[0]

    print([a.id for a in frame.atoms])

First we need to define new order of atoms.
For sake of simplicity let's number them in reverse order

.. py-exec::
    :context-id: reorder_atoms

    atoms = frame.atoms

    for i, a in enumerate(atoms):
        a.id = atoms.size - i

    # New atom ids:
    print([a.id for a in frame.atoms])


As you can see :ref:`id <Atom.id>` does not affect atom order in selections.

We can create a new Frame with altered atom order `within` residue.

.. py-exec::
    :context-id: reorder_atoms
    :discard-context:

    from pyxmolpp2 import Frame

    # create empty frame with index=0
    new_frame = Frame()

    for mol in frame.molecules:
        # create empty chain with same name
        new_mol = new_frame.add_molecule()
        new_mol.name = mol.name
        for residue in frame.residues:
            # create empty residue with same name and id
            new_residue = new_mol.add_residue()
            new_residue.name, new_residue.id = residue.name, residue.id
            for a in sorted(list(residue.atoms), key=lambda a: a.id):
                new_atom = new_residue.add_atom()
                new_atom.r, new_atom.id, new_atom.name = a.r, a.id, a.name

    # New frame atoms ids:
    print([a.id for a in new_frame.atoms])

.. block-info::
    Hierarchy defines order

    Note that frame order is superior to chain order, chain order is superior to residue order,
    residue order is superior to atom order. In other words children order follows parents order.
