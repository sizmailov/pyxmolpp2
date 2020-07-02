import pytest
import os

def test_read_frame():
    from pyxmolpp2 import PdbFile, UnitCell, Degrees

    cells = [
        UnitCell(36.633, 36.633, 79.254, Degrees(90), Degrees(90), Degrees(120.00)),
        UnitCell(50.840, 42.770, 28.950, Degrees(90), Degrees(90.00), Degrees(90.00)),
        UnitCell(52.323, 79.498, 52.406, Degrees(90), Degrees(90.14), Degrees(90.00)),
        UnitCell(25.037, 37.194, 49.217, Degrees(90), Degrees(90.00), Degrees(90.00)),
    ]

    for pdb_code, cell in zip(['1PGB', '1UBQ', '5BMG', '5BMH'], cells):
        frames = PdbFile(os.environ["TEST_DATA_PATH"] + f"/pdb/rcsb/{pdb_code}.pdb").frames()
        for frame in frames:
            for i in range(3):
                assert cell[i].distance(frame.cell[i]) == pytest.approx(0)
            assert frame.atoms.size > 0


def test_read_frames():
    from pyxmolpp2 import PdbFile
    import glob

    for filename in glob.glob(os.environ["TEST_DATA_PATH"] + "/pdb/rcsb/*.pdb"):
        frames = PdbFile(filename).frames()

        assert len(frames) > 0
        print(len(frames))


def test_read_non_existent_file():
    from pyxmolpp2 import PdbFile

    with pytest.raises(RuntimeError):
        PdbFile("does_not_exists.pdb")


def test_read_empty_file():
    from pyxmolpp2 import PdbFile
    import os

    assert len(PdbFile(os.devnull).frames()) == 0
