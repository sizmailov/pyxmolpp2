"""

.. |Atom| replace:: :py:class:`~pyxmolpp2.polymer.Atom`
.. |Residue| replace:: :py:class:`~pyxmolpp2.polymer.Residue`
.. |Chain| replace:: :py:class:`~pyxmolpp2.polymer.Chain`
.. |Frame| replace:: :py:class:`~pyxmolpp2.polymer.Frame`

.. |AtomSelection| replace:: :py:class:`~pyxmolpp2.polymer.AtomSelection`
.. |ResidueSelection| replace:: :py:class:`~pyxmolpp2.polymer.ResidueSelection`
.. |ChainSelection| replace:: :py:class:`~pyxmolpp2.polymer.ChainSelection`

.. |AtomPredicate| replace:: :py:class:`~pyxmolpp2.polymer.AtomPredicate`
.. |ResiduePredicate| replace:: :py:class:`~pyxmolpp2.polymer.ResiduePredicate`
.. |ChainPredicate| replace:: :py:class:`~pyxmolpp2.polymer.ChainPredicate`



Reorder atoms/residues
----------------------

In this example we will see how to reorder atoms/residues/chains

"""

import pyxmolpp2
import os

##############################################################################
# Let's create a frame to work with

pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

frame = pdb_file.get_frame()

# Original atom ids:
print([a.id for a in frame.asAtoms])

##############################################################################
# First we need to define new order of atoms.
# For sake of simplicity let's number them in reverse order

atoms = frame.asAtoms

for i, a in enumerate(atoms):
    a.id = atoms.size - i

# New atom ids:
print([a.id for a in frame.asAtoms])

##############################################################################
# As you can see `atom.id` does not affect atom order.
# To change that we need to construct new Frame with desired order of atoms:

from pyxmolpp2.polymer import Frame

new_frame = Frame(0)  # create empty frame with index=0

for chain in frame.asChains:
    new_chain = new_frame.emplace(chain.name)  # create empty chain with same name
    for residue in frame.asResidues:
        new_residue = new_chain.emplace(residue.name, residue.id)  # create empty residue with same name and id
        for a in sorted(list(residue.asAtoms), key=lambda a: a.id):
            new_atom = new_residue.emplace(a)  # create a copy of atom `a`

# New frame atoms ids:
print([a.id for a in new_frame.asAtoms])

