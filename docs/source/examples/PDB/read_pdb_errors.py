"""

PDB read with errors
--------------------

If your PDB is arbitrary formatted it might be successfully read but
the result might be unexpected.

Most likely you will get messed atom/residue ids, wrong coordinates or if you are lucky an Exception which will tell you
what is wrong with your PDB file.

Please see how to read non-standard, but well-formatted pdb file: :ref:`read-write-non-standard-pdb`

"""

import pyxmolpp2
import os

pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "trjtool/GB1/run00001.pdb")

# Define a PDB file with standard records fields layout
pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

##############################################################################
# Failed read:
frame = pdb_file.get_frame()
