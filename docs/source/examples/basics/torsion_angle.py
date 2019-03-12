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



Torsion Angles
--------------

"""

import pyxmolpp2
import os

##############################################################################
# Let's create a frame to work with

pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

frame = pdb_file.get_frame()

##############################################################################
# Standard torsion angles
# ^^^^^^^^^^^^^^^^^^^^^^^
#
# For standard protein residues angles can be constructed using `TorsionAngleFactory`:

from pyxmolpp2.polymer import TorsionAngleFactory
from pyxmolpp2.geometry import Degrees

residue48 = frame.asChains[0][48]
print(residue48)

psi_48 = TorsionAngleFactory.psi(residue48)
print(psi_48)
print(psi_48.value().degrees)

##############################################################################
# Note: Factory may return ``None`` if such angle does not exist:
print(TorsionAngleFactory.omega(frame.asResidues[0]))

##############################################################################
# Torsion angle allows to set a new one:

# All residues 49-76 are affected by this rotation
psi_48.set(Degrees(150))
print(psi_48.value().degrees)

##############################################################################
# Construction
# ^^^^^^^^^^^^
# Torsion angle constructor allow two forms:
#  1. Read-only torsion angle
#  2. Read-write torsion angle

from pyxmolpp2.polymer import TorsionAngle, AtomName, Atom

r1 = frame.asResidues[1]
r2 = frame.asResidues[2]

# Let's create a read-only phi of residue 2
phi_2_ro = TorsionAngle(r1[AtomName("C")],
                        r2[AtomName("N")],
                        r2[AtomName("CA")],
                        r2[AtomName("C")],
                        )

# Check against factory angle:
assert phi_2_ro.value().degrees == TorsionAngleFactory.phi(r2).value().degrees

##############################################################################
# Attempt to set TorsionAngle will lead to ``RuntimeError``:
try:
    phi_2_ro.set(Degrees(-130))
except RuntimeError as e:
    print(e)


##############################################################################
# We need a helper function which returns a selection of affected atoms
# by our torsion angle

def affected_phi_atoms(a: Atom, b: Atom, c: Atom, d: Atom):
    from pyxmolpp2.polymer import rId
    return a.chain.asResidues.filter(rId > a.rId).asAtoms


phi_2_rw = TorsionAngle(r1[AtomName("C")],
                        r2[AtomName("N")],
                        r2[AtomName("CA")],
                        r2[AtomName("C")],
                        affected_phi_atoms
                        )

phi_2_rw.set(Degrees(-130))

print(phi_2_ro.value().degrees)
print(phi_2_rw.value().degrees)


