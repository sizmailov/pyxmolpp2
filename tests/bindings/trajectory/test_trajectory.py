import pytest

def test_traj_iteration():

    from pyxmolpp2.pdb import PdbFile, AlteredPdbRecords, StandardPdbRecords, RecordName, FieldName
    from pyxmolpp2.trjtool import DatFile
    from pyxmolpp2.trajectory import Trajectory

    records = AlteredPdbRecords(StandardPdbRecords.instance())

    records.alter_record(RecordName("ATOM"), FieldName("serial"), [7, 12])

    frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb",records).get_frame()

    assert frame.asAtoms.size > 0

    datfile1 = DatFile("tests_dataset/trjtool/GB1/run00001.dat")
    datfile2 = DatFile("tests_dataset/trjtool/GB1/run00002.dat")

    trj = Trajectory(frame, True)

    trj.push_trajectory_portion(datfile1)
    trj.push_trajectory_portion(datfile2)

    assert trj.size == datfile1.n_frames + datfile2.n_frames

    n = 0
    stride = 50
    for f in trj[::stride]:
        r = f.asAtoms[0].r
        n +=1

    assert trj.size//stride == n

    n = 0
    stride = 1
    ats = frame.asResidues[0].asAtoms
    trj.set_update_list(ats)
    for f in trj[::stride]:
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
        frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb", records).get_frame()
        frame.asAtoms[0].name=AtomName("XX")
        trj = Trajectory(frame, True)
        trj.push_trajectory_portion(datfile1)

    with pytest.raises(TrajectoryException):
        frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb",records).get_frame()
        frame.asAtoms[0].delete()
        trj = Trajectory(frame, True)
        trj.push_trajectory_portion(datfile1)


def test_traj_size():

    from pyxmolpp2.pdb import PdbFile, AlteredPdbRecords, StandardPdbRecords, RecordName, FieldName

    from pyxmolpp2.trjtool import DatFile
    from pyxmolpp2.trajectory import Trajectory


    records = AlteredPdbRecords(StandardPdbRecords.instance())
    records.alter_record(RecordName("ATOM"), FieldName("serial"), [7,12])

    frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb",records).get_frame()

    assert frame.asAtoms.size > 0

    trj = Trajectory(frame, True)
    trj.push_trajectory_portion(DatFile("tests_dataset/trjtool/GB1/run00001.dat"))
    assert len(trj[0:10]) == 10
    assert len(trj[0:10:10]) == 1
    assert len(trj[0:10:100]) == 1
    assert len(trj[0:100:10]) == 10
    assert len(trj[0:100:-10]) == 0
    assert len(trj[100:0:-1]) == 100
    assert len(trj[0:100:-1]) == 0


def test_trajectory_integer_indexing():

    from pyxmolpp2.pdb import PdbFile, AlteredPdbRecords, StandardPdbRecords, RecordName, FieldName

    from pyxmolpp2.trjtool import DatFile
    from pyxmolpp2.trajectory import Trajectory


    records = AlteredPdbRecords(StandardPdbRecords.instance())
    records.alter_record(RecordName("ATOM"), FieldName("serial"), [7,12])

    frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb",records).get_frame()

    trj = Trajectory(frame, True)
    trj.push_trajectory_portion(DatFile("tests_dataset/trjtool/GB1/run00001.dat"))

    n = trj.size


    trj[-n]
    trj[n-1]

    with pytest.raises(IndexError):
        trj[n]

    with pytest.raises(IndexError):
        trj[-n-1]

    frame1 = trj[0]
    frame2 = trj[-n]

    assert frame1.index == frame2.index
