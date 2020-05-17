PDB read/write
^^^^^^^^^^^^^^

There are many flavours of PDB managed differently by different programs.
By default ``pyxmolpp2`` uses PDB 3.0 standard convention.
For example, reading ``tleap``-like ``.pdb`` file as a standard one results to an error:

.. py-exec::
    :context-id: pdb_read_write
    :raises: PdbException

    import os
    import pyxmolpp2

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "trjtool/GB1/run00001.pdb")
    pdb_file = pyxmolpp2.pdb.PdbFile(pdb_filename)

    frame = pdb_file.get_frame()


``pyxmolpp2`` supports reading/writing non-standard PDB files.
To read such file you need to specify a set of new records (usually based on :ref:`pdb.StandardPdbRecords` records).
In next example we will create ``altered_records`` which describe Amber-flavoured ``.pdb``:

.. py-exec::
    :context-id:  pdb_read_write

    from pyxmolpp2.pdb import (AlteredPdbRecords, StandardPdbRecords,
                               FieldName, RecordName, PdbFile)

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
    print("PDB contains %d MODEL(S): " % len(frames))
    for frame in frames:
        print("\tMODEL #%d contains %d molecules with %d atoms in total" % (
            frame.index,
            frame.asChains.size,
            frame.asAtoms.size
        ))

Full set of available :ref:`pdb.RecordName`,  :ref:`pdb.FiledName` and standard fields you can find `here <https://github.com/sizmailov/pyxmolpp2/blob/master/src/xmol/pdb/BundledPDBRecordTypesBaseInit.cpp#L32>`_

All classes :ref:`Frame` to :ref:`Atom` have ``.to_pdb`` method which writes corresponding structure in pdb format.
It has optional parameter to specify format

.. py-exec::
    :context-id:  pdb_read_write

    import io
    import sys

    buffer = io.StringIO()
    frames[0].to_pdb(buffer)
    buffer.seek(0)
    print("Standard PDB format:")
    print(''.join(buffer.readlines()[:3]))

    buffer = io.StringIO()
    frames[0].to_pdb(buffer, altered_records)
    buffer.seek(0)
    print("Non-standard PDB format:")
    print(''.join(buffer.readlines()[:3]))

    # Selections has .to_pdb method too!
    print("First residue")
    frames[0].asResidues[:1].to_pdb(sys.stdout)


To write multiple frames to single pdb you can use similar snippet:

.. py-exec::
    :context-id:  pdb_read_write
    :discard-context:

    with open("/dev/null", "w") as output:
        for frame in frames:
            frame.to_pdb(output)
            output.write("ENDMDL\n")


