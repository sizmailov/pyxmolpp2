"""

.. _read-write-non-standard-pdb:

Non-standard PDB io
-------------------

``pyxmolpp2`` supports reading/writing non-standard PDB files. To read such file you need to specify
a set of new records (usually based on :py:class:`~pyxmolpp2.pdb.StandardPdbRecords` records).

Full set of available :py:class:`~pyxmolpp2.pdb.RecordName`,  :py:class:`~pyxmolpp2.pdb.FiledName` and standard values you can find `here <https://github.com/sizmailov/pyxmolpp2/blob/master/src/xmol/pdb/BundledPDBRecordTypesBaseInit.cpp#L32>`_


"""

import pyxmolpp2
from pyxmolpp2.pdb import AlteredPdbRecords, StandardPdbRecords, FieldName, RecordName, PdbFile
import os

# Create PDB records description based on standard records
altered_records = AlteredPdbRecords(pyxmolpp2.pdb.StandardPdbRecords.instance())

# Expand ATOM.serial record to columns 7-12
altered_records.alter_record(RecordName("ATOM"), FieldName("serial"), [7, 12])

pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "trjtool/GB1/run00001.pdb")

# Define a PDB file with altered records fields layout
pdb_file = PdbFile(pdb_filename, altered_records)

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
frames[0].to_pdb("out.pdb", altered_records)
