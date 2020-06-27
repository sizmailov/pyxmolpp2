PDB files
#########

There are many flavours of PDB managed differently by different programs.
By default ``pyxmolpp2`` uses AMBER-compatible convention.
For example, reading ``tleap``-like ``.pdb`` file as a standard one results to an error:

.. py-exec::
    :context-id: pdb_read_write
    :raises: RuntimeError

    import os
    from pyxmolpp2 import PdbFile

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "trjtool/GB1/run00001.pdb")
    pdb_file = PdbFile(pdb_filename, PdbFile.STANDARD_V3)


To write multiple frames to a single pdb you can use next snippet:

.. py-exec::
    :context-id:  pdb_read_write
    :discard-context:

    pdb_file = PdbFile(pdb_filename)
    frames = pdb_file.frames()

    with open("/dev/null", "w") as output:
        for i, frame in enumerate(frames):
            output.write("MODEL    {i:4d}\n")
            frame.to_pdb(output)
            output.write("ENDMDL\n")


