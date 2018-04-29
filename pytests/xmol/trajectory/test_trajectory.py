import pytest

def test_traj_iteration():

    from pyxmolpp2.pdb import PdbFile, AlteredPdbRecords, StandardPdbRecords, RecordName, FieldName
    from pyxmolpp2.trjtool import DatFile
    from pyxmolpp2.trajectory import Trajectory

    records = AlteredPdbRecords(StandardPdbRecords.instance())

    records.alter_record(RecordName("ATOM"), FieldName("serial"), [7,12])

    frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb").get_frame(records)

    assert frame.asAtoms > 0

    datfile1 = DatFile("tests_dataset/trjtool/GB1/run00001.dat")
    datfile2 = DatFile("tests_dataset/trjtool/GB1/run00002.dat")

    trj = Trajectory(frame, True)

    trj.push_trajectory_portion(datfile1)
    trj.push_trajectory_portion(datfile2)

    assert trj.size == datfile1.n_frames+ datfile2.n_frames

    n = 0
    stride = 50
    for frame in trj[::stride]:
        r = frame.asAtoms[0].r
        print(r.x,r.y,r.z)
        n +=1

    assert trj.size//stride == n


def test_traj_exceptions():

    from pyxmolpp2.pdb import PdbFile, AlteredPdbRecords, StandardPdbRecords, RecordName, FieldName
    from pyxmolpp2.polymer import AtomName
    from pyxmolpp2.trjtool import DatFile
    from pyxmolpp2.trajectory import Trajectory, TrajectoryException

    records = AlteredPdbRecords(StandardPdbRecords.instance())
    records.alter_record(RecordName("ATOM"), FieldName("serial"), [7,12])

    datfile1 = DatFile("tests_dataset/trjtool/GB1/run00001.dat")

    with pytest.raises(TrajectoryException):
        frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb").get_frame(records)
        frame.asAtoms[0].name=AtomName("XX")
        trj = Trajectory(frame, True)
        trj.push_trajectory_portion(datfile1)

    with pytest.raises(TrajectoryException):
        frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb").get_frame(records)
        frame.asAtoms[0].delete()
        trj = Trajectory(frame, True)
        trj.push_trajectory_portion(datfile1)


