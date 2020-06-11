import pytest


def test_read_frame():
    from pyxmolpp2.v1 import PdbFile
    import glob

    for filename in glob.glob("tests_dataset/pdb/rcsb/*.pdb"):
        frame = PdbFile(filename).frames()[0]
        assert frame.atoms.size > 0


def test_read_frames():
    from pyxmolpp2.v1 import PdbFile
    import glob

    for filename in glob.glob("tests_dataset/pdb/rcsb/*.pdb"):
        frames = PdbFile(filename).frames()

        assert len(frames) > 0
        print(len(frames))


def test_read_non_existent_file():
    from pyxmolpp2.v1 import PdbFile

    with pytest.raises(RuntimeError):
        PdbFile("does_not_exists.pdb")


def test_read_empty_file():
    from pyxmolpp2.v1 import PdbFile
    import os

    assert len(PdbFile(os.devnull).frames()) == 0

# todo: move to another test set
# def test_rmsd():
#     from pyxmolpp2.v1 PdbFile, import calc_rmsd, calc_alignment, calc_geom_center
#     import glob
#
#     filenames = sorted(glob.glob("tests_dataset/pdb/rcsb/*.pdb"))
#
#     for i in range(len(filenames) - 1):
#         frame_1 = PdbFile(filenames[i]).get_frame()
#         residues_1 = [a.rId for a in frame_1.asChains[0].asAtoms.filter(lambda a: a.name.str == "CA")]
#
#         for j in range(i + 1, len(filenames)):
#             frame_2 = PdbFile(filenames[j]).get_frame()
#             residues_2 = [a.rId for a in frame_2.asChains[0].asAtoms.filter(lambda a: a.name.str == "CA")]
#             common = set(residues_1) & set(residues_2)
#             ats1 = frame_1.asChains[0].asAtoms.filter(
#                 lambda a: (a.id == a.residue[AtomName("CA")].id) and a.rId in common)
#             ats2 = frame_2.asChains[0].asAtoms.filter(
#                 lambda a: (a.id == a.residue[AtomName("CA")].id) and a.rId in common)
#
#             assert ats1.size == len(common)
#             assert ats2.size == len(common)
#             assert ats1.size == ats2.size
#
#             assert len(ats1) > 3
#             assert len(ats2) > 3
#             aligment = calc_alignment(ats1.toCoords, ats2.toCoords)
#             not_aligned_rmsd = calc_rmsd(ats1.toCoords, ats2.toCoords)
#             aligned_rmsd = calc_rmsd(ats1.toCoords, ats2.toCoords, aligment)
#
#             # print(filenames[i],filenames[j])
#             # print("Not aliged:", not_aligned_rmsd)
#             # print("   aligned:", aligned_rmsd)
#
#         #     print(frame[i].asAtoms,frame[j].asAtoms)

# todo: move to another test set
# def test_torsions():
#     from pyxmolpp2.v1 import TorsionAngleFactory, OutOfRangeResidue
#     from pyxmolpp2.pdb import PdbFile
#     from pyxmolpp2.geometry import Degrees
#     import glob
#     filenames = sorted(glob.glob("tests_dataset/pdb/rcsb/*.pdb"))
#
#     for filename in filenames:
#         frame = PdbFile(filename).get_frame()
#         for r in frame.asResidues:
#             try:
#                 angles = [
#                     TorsionAngleFactory.phi(r),
#                     TorsionAngleFactory.psi(r),
#                     TorsionAngleFactory.omega(r),
#                     TorsionAngleFactory.chi1(r),
#                     TorsionAngleFactory.chi2(r),
#                     TorsionAngleFactory.chi3(r),
#                     TorsionAngleFactory.chi4(r),
#                     TorsionAngleFactory.chi5(r),
#                 ]
#                 for angle in angles:
#                     if angle:
#                         try:
#                             value = angle.value()
#                             angle.set(value + Degrees(30), Degrees(0))
#                         except Exception:
#                             pass
#             except OutOfRangeResidue:
#                 pass
