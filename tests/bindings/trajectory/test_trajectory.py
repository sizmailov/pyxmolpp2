import pytest
import os

def test_traj_iteration():
    from pyxmolpp2 import PdbFile, TrjtoolDatFile as DatFile, Trajectory
    frame = PdbFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.pdb").frames()[0]

    assert frame.atoms.size > 0

    datfile1 = DatFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.dat")
    datfile2 = DatFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00002.dat")

    trj = Trajectory(frame)

    trj.extend(datfile1)
    trj.extend(datfile2)

    assert trj.n_frames == datfile1.n_frames() + datfile2.n_frames()

    n = 0
    stride = 50
    for f in trj[::stride]:
        r = f.atoms[0].r
        n += 1

    assert trj.n_frames // stride == n


@pytest.mark.skip("Consistency checks are not implemented")
def test_traj_exceptions():
    from pyxmolpp2 import PdbFile, TrjtoolDatFile as DatFile, Trajectory

    datfile1 = DatFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.dat")

    # atom name is different
    with pytest.raises(RuntimeError):
        frame = PdbFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.pdb").frames()[0]
        frame.atoms[0].name = "XX"
        trj = Trajectory(frame)
        trj.extend(datfile1)

    # atom number of atoms is different
    with pytest.raises(RuntimeError):
        frame = PdbFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.pdb").frames()[0]
        frame.atoms[0].erase()
        trj = Trajectory(frame)
        trj.extend(datfile1)


def test_traj_size():
    from pyxmolpp2 import PdbFile, TrjtoolDatFile as DatFile, Trajectory
    frame = PdbFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.pdb").frames()[0]

    assert frame.atoms.size > 0

    trj = Trajectory(frame)
    trj.extend(DatFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.dat"))

    assert sum([1 for _ in trj[0:10]]) == 10
    assert sum([1 for _ in trj[0:10:10]]) == 1
    assert sum([1 for _ in trj[0:10:100]]) == 1
    assert sum([1 for _ in trj[0:100:10]]) == 10

    assert sum([1 for _ in trj[0:-1000]]) == 0
    assert sum([1 for _ in trj[-100:]]) == 100
    assert sum([1 for _ in trj[:-100]]) == 900

    with pytest.raises(TypeError):
        trj[::-1]


def test_trajectory_integer_indexing():
    from pyxmolpp2 import PdbFile, TrjtoolDatFile as DatFile, Trajectory

    frame = PdbFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.pdb").frames()[0]

    trj = Trajectory(frame)
    trj.extend(DatFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.dat"))

    n = trj.n_frames

    trj[-n]
    trj[n - 1]

    with pytest.raises(IndexError):
        trj[n]

    with pytest.raises(IndexError):
        trj[-n - 1]

    frame1 = trj[0]
    frame2 = trj[-n]
    frame3 = trj[0]

    assert frame1 != frame2
    assert frame1 != frame3
