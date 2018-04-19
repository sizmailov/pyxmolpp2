import pytest


def test_read_frame():
    from pyxmolpp2.pdb import PdbFile
    import glob



    for filename in glob.glob("tests_dataset/pdb/rcsb/*.pdb"):

        frame = PdbFile(filename).get_frame()

        assert frame.asAtoms > 0
        print (frame.asAtoms)