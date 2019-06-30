"""

Write multiple frames to pdb
----------------------------

"""

import pyxmolpp2
from pyxmolpp2.geometry import Translation3d, XYZ
import os

pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1PGB.pdb")

# Read standard PDB file
pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

# Read all frames (MODELS in PDB terms)
frames = pdb_file.get_frames()

# Place all frames geometric center at origin
for frame in frames:
    frame.asAtoms.transform(Translation3d(-frame.asAtoms.geom_center()))

# Write all frames to one pdb file
with open("out.pdb", "w") as output:
    for frame in frames:
        frame.to_pdb(output)
        output.write("ENDMDL\n")
