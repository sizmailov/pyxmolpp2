import pytest

def test_read_trjtool():

    from pyxmolpp2.pdb import PdbFile, AlteredPdbRecords, StandardPdbRecords, RecordName, FieldName
    from pyxmolpp2.trjtool import DatFile

    records = AlteredPdbRecords(StandardPdbRecords.instance())

    records.alter_record(RecordName("ATOM"), FieldName("serial"), [7,12])

    frame = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb",records).get_frame()

    assert frame.asAtoms.size > 0

    datfile = DatFile("tests_dataset/trjtool/GB1/run00001.dat")

    assert datfile.n_frames == 1000
    assert datfile.match(frame.asAtoms)
    assert datfile.n_atoms_per_frame == frame.asAtoms.size

    datfile.set_coordinates(0, frame.asAtoms, list(range(frame.asAtoms.size)))


def test_raise_file_does_not_exists():
    from pyxmolpp2.trjtool import DatFile, TrjtoolException

    with pytest.raises(TrjtoolException):
        DatFile("does_not_exists.dat")


def test_raise_file_is_empty():
    from pyxmolpp2.trjtool import DatFile, unexpected_eof
    import os
    with pytest.raises(unexpected_eof):
        DatFile(os.devnull)
