"""

.. _examples.sasa_calc:

Solvent Accessible Solvent Area (SASA)
--------------------------------------

ASA is important measure of protein molecule.

"""

import pyxmolpp2
import os
import numpy as np
from timeit import default_timer as timer

##############################################################################
# Let's create a frame to work with

pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)
frame = pdb_file.get_frame()

##############################################################################
# We need to set VDW radii for all atoms
# Let's do it according to the next table

vdw_table = {
    "C": 1.70,
    "O": 1.52,
    "S": 1.80,
    "N": 1.55,
    "H": 1.2,
}

atoms = frame.asAtoms

# First letter of atom name determinate it's element
vdw_radii = np.array([vdw_table[a.name.str[0]] for a in atoms])

# Solvent radius for water is usually 1.4 A
solvent_radius = 1.4

# Measure time spent on SASA calculation
t_start = timer()
sasa_per_atom = pyxmolpp2.geometry.calc_sasa(atoms.toCoords,
                                             vdw_radii,
                                             solvent_radius,
                                             n_samples=20
                                             )
t_end = timer()

# Print all sasa values
print("Per-atom SASA")
for sasa, a in zip(sasa_per_atom, atoms):
    print("{:3d}-{:3s}.{:4s} {:7.2f}".format(a.rId.serial, a.rName.str, a.name.str, sasa))

##############################################################################

print("Total SASA:", sum(sasa_per_atom))
print("Elapsed time: %.3f s"%(t_end - t_start))

##############################################################################
# Oftentimes it's required to calculate SASA of particular site
#
# To avoid unwanted calculation one can provide indices of atoms of interest:
#
from pyxmolpp2.polymer import rId

index = atoms.index(rId <= 5) # take indices of first 5 residues

t_start = timer()
first_5_residues_sasa =  pyxmolpp2.geometry.calc_sasa(atoms.toCoords,
                                                      vdw_radii,
                                                      solvent_radius,
                                                      index,
                                                      n_samples=20
                                                      )

t_end = timer()

# Print all sasa values
print("Per-atom SASA")
for sasa, a in zip(first_5_residues_sasa, atoms[index]):
    print("{:3d}-{:3s}.{:4s} {:7.2f}".format(a.rId.serial, a.rName.str, a.name.str, sasa))
##############################################################################
print("1-5 residues SASA (20 samples):", sum(first_5_residues_sasa))
print("Elapsed time: %.3f s"%(t_end - t_start))

##############################################################################
t_start = timer()
first_5_residues_sasa =  pyxmolpp2.geometry.calc_sasa(atoms.toCoords,
                                                      vdw_radii,
                                                      solvent_radius,
                                                      index,
                                                      n_samples=200
                                                      )

t_end = timer()
print("1-5 residues SASA (200 samples):", sum(first_5_residues_sasa))
print("Elapsed time: %.3f s"%(t_end - t_start))