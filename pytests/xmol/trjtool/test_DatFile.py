import pytest

def test_read_trjtool():

    from pyxmolpp2.pdb import PdbFile
    from pyxmolpp2.trjtool import DatFile

    frame = PdbFile("tests_dataset/trjtool/GB1/run00001-mod.pdb").get_frame()

    assert frame.asAtoms > 0

    datfile = DatFile("tests_dataset/trjtool/GB1/run00001.dat")

    assert datfile.n_frames() == 1000
    assert datfile.match(frame.asAtoms)
    assert datfile.n_atoms_per_frame() == frame.asAtoms.size


