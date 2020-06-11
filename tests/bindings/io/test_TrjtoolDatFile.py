import pytest

def test_read_trjtool():

    from pyxmolpp2.v1 import PdbFile, TrjtoolDatFile

    frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb").frames()[0]

    assert frame.atoms.size > 0

    datfile = TrjtoolDatFile("tests_dataset/trjtool/GB1/run00001.dat")

    assert datfile.n_frames() == 1000
    assert datfile.n_atoms() == frame.atoms.size

    datfile.read_coordinates(0, frame.coords)


def test_raise_file_does_not_exists():
    from pyxmolpp2.trjtool import DatFile, TrjtoolException

    with pytest.raises(TrjtoolException):
        DatFile("does_not_exists.dat")


def test_raise_file_is_empty():
    from pyxmolpp2.trjtool import DatFile, unexpected_eof
    import os
    with pytest.raises(unexpected_eof):
        DatFile(os.devnull)
