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
    for frame in trj:
        r = frame.asAtoms[0].r
        print(r.x,r.y,r.z)
        n +=1

    assert trj.size == n


