import pytest

def test_read_trjtool():

    from pyxmolpp2.pdb import PdbFile, AlteredPdbRecords, StandardPdbRecords, RecordName, FieldName
    from pyxmolpp2.trjtool import DatFile

    records = AlteredPdbRecords(StandardPdbRecords.instance())

    records.alter_record(RecordName("ATOM"), FieldName("serial"), [7,12])

    frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb").get_frame(records)

    assert frame.asAtoms > 0

    datfile = DatFile("tests_dataset/trjtool/GB1/run00001.dat")

    assert datfile.n_frames() == 1000
    assert datfile.match(frame.asAtoms)
    assert datfile.n_atoms_per_frame() == frame.asAtoms.size

    datfile.set_coordinates(0, frame.asAtoms)


