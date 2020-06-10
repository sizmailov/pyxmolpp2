import pytest


def test_read_trjtool():
    from pyxmolpp2.pdb import PdbFile, AlteredPdbRecords, StandardPdbRecords, RecordName, FieldName
    from pyxmolpp2.amber import NetCDFTrajectoryFile

    records = AlteredPdbRecords(StandardPdbRecords.instance())

    records.alter_record(RecordName("ATOM"), FieldName("serial"), [7, 12])

    frame = PdbFile("tests_dataset/amber/GB1_F30C_MTSL/box.pdb", records).get_frame()

    assert frame.asAtoms.size > 0

    datfile = NetCDFTrajectoryFile("tests_dataset/amber/GB1_F30C_MTSL/GB1_F30C_MTSL_10_frames.nc")

    assert datfile.n_frames == 10
    assert datfile.match(frame.asAtoms)
    assert datfile.n_atoms_per_frame == frame.asAtoms.size

    datfile.set_coordinates(0, frame.asAtoms, list(range(frame.asAtoms.size)))
