import pytest
import os

def test_read_frame():
    from pyxmolpp2 import PdbFile
    import glob

    for filename in glob.glob(os.environ["TEST_DATA_PATH"] + "/pdb/rcsb/*.pdb"):
        frame = PdbFile(filename).frames()[0]
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
