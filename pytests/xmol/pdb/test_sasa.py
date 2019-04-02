import pytest


def test_read_frame():
    from pyxmolpp2.pdb import PdbFile
    from pyxmolpp2.geometry import calc_sasa
    import glob

    for filename in glob.glob("tests_dataset/pdb/rcsb/*.pdb"):
        frame = PdbFile(filename).get_frame()
        ats = frame.asAtoms
        radii = [1.0] * ats.size
        print(filename, sum(calc_sasa(ats.toCoords, radii, 0.0)))

