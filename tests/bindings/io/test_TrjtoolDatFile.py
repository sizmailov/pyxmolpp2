import pytest
import os

def test_read_trjtool():
    from pyxmolpp2 import PdbFile, TrjtoolDatFile

    frame = PdbFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.pdb").frames()[0]

    assert frame.atoms.size > 0

    datfile = TrjtoolDatFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.dat")

    assert datfile.n_frames() == 1000
    assert datfile.n_atoms() == frame.atoms.size
    datfile.advance(0)
    datfile.read_coordinates(0, frame.coords)


def test_raise_file_does_not_exists():
    from pyxmolpp2 import TrjtoolDatFile

    with pytest.raises(RuntimeError):
        TrjtoolDatFile("does_not_exists.dat")


def test_raise_file_is_empty():
    from pyxmolpp2 import TrjtoolDatFile
    import os
    with pytest.raises(RuntimeError):
        TrjtoolDatFile(os.devnull)
