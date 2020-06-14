import pytest


def test_read_netcdf():
    from pyxmolpp2 import PdbFile, AmberNetCDF

    frame = PdbFile("tests_dataset/amber/GB1_F30C_MTSL/box.pdb").frames()[0]

    assert frame.atoms.size > 0

    datfile = AmberNetCDF("tests_dataset/amber/GB1_F30C_MTSL/GB1_F30C_MTSL_10_frames.nc")

    assert datfile.n_frames() == 10
    assert datfile.n_atoms() == frame.atoms.size

    datfile.read_coordinates(0, frame.coords)
