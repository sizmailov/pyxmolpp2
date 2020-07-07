import pytest
import os
import os

def test_read_one_file():
    from pyxmolpp2 import PdbFile, GromacsXtcFile

    frame = PdbFile(os.environ["TEST_DATA_PATH"] + "/gromacs/xtc/1am7_protein.pdb").frames()[0]

    assert frame.atoms.size > 0

    inp = GromacsXtcFile(os.environ["TEST_DATA_PATH"] + "/gromacs/xtc/1am7_corrected.xtc", 51)

    assert inp.n_frames() == 51
    assert inp.n_atoms() == frame.atoms.size
    inp.advance(0)
    inp.read_frame(0, frame)


def test_writer():
    from pyxmolpp2 import PdbFile, XtcWriter, Translation, XYZ

    xtc_writer = XtcWriter("test.xtc", 1000)
    frame = PdbFile(os.environ["TEST_DATA_PATH"] + "/gromacs/xtc/1am7_protein.pdb").frames()[0]
    for i in range(10):
        frame.cell.scale_by(1.2)
        frame.coords.apply(Translation(XYZ(1, 0, 0)))
        xtc_writer.write(frame)
    del xtc_writer
    os.remove("test.xtc")
