import pytest


def test_read_frame():
    from pyxmolpp2.pdb import PdbFile
    import glob



    for filename in glob.glob("tests_dataset/pdb/rcsb/*.pdb"):

        frame = PdbFile(filename).get_frame()

        assert frame.asAtoms > 0
        # print (frame.asAtoms)

def test_rmsd():
    from pyxmolpp2.polymer import AtomName
    from pyxmolpp2.pdb import PdbFile
    from pyxmolpp2.geometry import calc_rmsd,calc_alignment,calc_geom_center
    import glob

    filenames = sorted(glob.glob("tests_dataset/pdb/rcsb/*.pdb"))


    for i in range(len(filenames)-1):
        frame_1 = PdbFile(filenames[i]).get_frame()
        residues_1 = [ a.rId for a in frame_1.asChains[0].asAtoms.filter(lambda a: a.name.str == "CA") ]

        for j in range(i+1,len(filenames)):
            frame_2 = PdbFile(filenames[j]).get_frame()
            residues_2 = [ a.rId for a in frame_2.asChains[0].asAtoms.filter(lambda a: a.name.str == "CA") ]
            common = set(residues_1) & set(residues_2)
            ats1 = frame_1.asChains[0].asAtoms.filter(lambda a: (a.id == a.residue[AtomName("CA")].id) and a.rId in common)
            ats2 = frame_2.asChains[0].asAtoms.filter(lambda a: (a.id == a.residue[AtomName("CA")].id) and a.rId in common)


            assert ats1.size == len(common)
            assert ats2.size == len(common)
            assert ats1.size == ats2.size

            assert len(ats1) > 3
            assert len(ats2) > 3
            aligment = calc_alignment(ats1.toCoords,ats2.toCoords)
            print(filenames[i],filenames[j])
            print("Not aliged:", calc_rmsd(ats1.toCoords,ats2.toCoords))
            print("   aligned:", calc_rmsd(ats1.toCoords,ats2.toCoords,aligment))

        #     print(frame[i].asAtoms,frame[j].asAtoms)




