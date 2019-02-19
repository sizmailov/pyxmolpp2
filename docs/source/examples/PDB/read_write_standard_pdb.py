"""

Standard PDB io
---------------

"""

import pyxmolpp2
import os

pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1PGB.pdb")

# Define a PDB file with standard records fields layout
pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

# Read all frames (i.e. MODELS) from PDB
frames = pdb_file.get_frames()


# Print some info about frames
print("PDB contains %d MODELS: " % len(frames))
for frame in frames:
    print("\tMODEL #%d contains %d molecules, total %d atoms" % (
        frame.index,
        frame.asChains.size,
        frame.asAtoms.size
    ))


# Write first model to PDB file using altered records
frames[0].to_pdb("out.pdb")
