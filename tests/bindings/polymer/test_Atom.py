import pytest
import os

def test_Frame():
    from pyxmolpp2.v1 import Frame, ResidueId, XYZ

    f = Frame()
    c = f.add_molecule()
    c.name = "A"
    r = c.add_residue()
    r.name = "LYS"
    r.id = ResidueId(1)
    a = r.add_atom()
    a.name = "CA"
    a.id = 1
    a.r = XYZ(1, 2, 3)

    assert a.residue == r
    assert a.molecule == c
    assert a.frame == f

    assert r.atoms[0] == a
    assert c.atoms[0] == a
    assert f.atoms[0] == a

    assert r.atoms.residues[0] == r
    assert c.atoms.residues[0] == r
    assert f.atoms.residues[0] == r

    assert r.atoms.residues.molecules[0] == c
    assert c.atoms.residues.molecules[0] == c
    assert f.atoms.residues.molecules[0] == c

    assert r.molecule == c
    assert r.frame == f

    assert c.frame == f

    assert c.residues[0] == r
    assert f.residues[0] == r

    assert f.molecules[0] == c


def make_polyglycine(chain_lengths):
    from pyxmolpp2.v1 import Frame, XYZ, ResidueId

    aid = 1
    rid = 1
    frame = Frame()
    for chainId, N in chain_lengths:
        c = frame.add_molecule()
        c.name = chainId
        for i in range(N):
            r = c.add_residue()
            r.name = "GLY"
            r.id = ResidueId(rid)

            rid += 1
            for aname in ["N", "H", "CA", "HA2", "HA3", "C", "O"]:
                a = r.add_atom()
                a.name = aname
                a.id = aid
                a.r = XYZ(1, 2, 3)
                aid += 1

    return frame


@pytest.mark.skipif("PYXMOLPP_RUN_PERFORMANCE_TESTS" not in os.environ,
                    reason="Run performance tests only in release mode")
def test_big_construction():
    from timeit import default_timer as timer

    for n in [10,100,1000,2000]:
        start = timer()
        frame = make_polyglycine([("A", n)])
        end = timer()
        assert frame.atoms.size == n * 7
        #print("%5d  %.5f"%(n,end-start))


def test_filters():
    n = 100
    frame = make_polyglycine([("A", n)])

    # atom filters
    assert frame.atoms.filter(lambda a: a.molecule.name == "A").size == n * 7
    assert frame.atoms.filter(lambda a: a.name == "CA").size == n
    assert frame.atoms.filter(lambda a: a.name == "N").size == n
    assert frame.atoms.filter(lambda a: a.name == "XXXX").size == 0

    # residue filters

    assert frame.residues.filter(lambda r: r.name == "GLY").size == n
    assert frame.residues.filter(lambda r: r.name == "LYS").size == 0
    assert frame.residues.filter(lambda r: r.id == 1).size == 1
    assert frame.residues.filter(lambda r: r.size == 1).size == 0

    # molecule filters
    assert frame.molecules.filter(lambda c: c.name == "1").size == 0
    assert frame.molecules.filter(lambda c: c.name == "A").size == 1
    assert frame.molecules.filter(lambda c: c.size > 2).size == 1

def test_iterable():
    frame = make_polyglycine([("A", 10)])

    assert len(list(frame.molecules)) == frame.molecules.size
    assert len(list(frame.residues)) == frame.residues.size
    assert len(list(frame.atoms)) == frame.atoms.size

def test_repr():
    frame = make_polyglycine([("A", 10)])

    str(frame)
    str(frame.molecules[0].name)
    str(frame.residues[0].name)
    str(frame.atoms[0].name)
    str([frame.molecules[0].name,
         frame.residues[0].name,
         frame.atoms[0].name]
        )

    str(frame.molecules[0])
    str(frame.residues[0])
    str(frame.atoms[0])

    str(frame.molecules)
    str(frame.residues)
    str(frame.atoms)





def test_assignment():
    from pyxmolpp2.v1 import XYZ
    frame = make_polyglycine([("A", 1)])
    for a in frame.atoms:
        # print(a,a.r)
        pass

    def translate_by_dr(a):
        # print(a,a.r)
        a.r = a.r + XYZ(1, 2, 3)

    for a in frame.atoms:
        translate_by_dr(a)

    assert frame.atoms[0].r.x == 1 * 2
    assert frame.atoms[0].r.y == 2 * 2
    assert frame.atoms[0].r.z == 3 * 2


def test_selection_exceptions():
    zombie_selection = make_polyglycine([("A", 1)]).atoms
    with pytest.raises(RuntimeError):
        for a in zombie_selection:
            pass

    zombie_selection = make_polyglycine([("A", 1)]).residues
    with pytest.raises(RuntimeError):
        for a in zombie_selection:
            pass

    zombie_selection = make_polyglycine([("A", 1)]).molecules
    with pytest.raises(RuntimeError):
        for a in zombie_selection:
            pass

def test_deleted_element_access_exceptions():

    frame = make_polyglycine([("A", 1)])
    sel = frame.atoms
    sel[0].delete()
    with pytest.raises(RuntimeError):
        for a in sel:
            pass

    frame = make_polyglycine([("A", 2)])
    sel = frame.residues
    sel[0].delete()
    with pytest.raises(RuntimeError):
        for a in sel:
            pass

    frame = make_polyglycine([("A", 2)] * 2)
    sel = frame.molecules
    sel[0].delete()
    with pytest.raises(RuntimeError):
        for a in sel:
            pass



def test_range_exceptions():
    frame = make_polyglycine([("A",1),("B",4),("C",3)])

    # with pytest.raises(OutOfRangeFrame):
    #     x = frame[3]

    # with pytest.raises(OutOfRangeChain):
    #     x = frame.molecules[0][3]

    with pytest.raises(RuntimeError):
        x = frame.residues[0]["ca"]

    assert frame.residues[0]["CA"].name == "CA"

    natoms = frame.atoms.size
    nresidues = frame.residues.size
    nchains = frame.molecules.size

    frame.atoms[0]
    frame.atoms[-natoms]
    frame.residues[0]
    frame.residues[-nresidues]
    frame.molecules[0]
    frame.molecules[-nchains]

    with pytest.raises(RuntimeError):
        x = frame.atoms[natoms]
    with pytest.raises(RuntimeError):
        x = frame.atoms[-natoms - 1]

    with pytest.raises(RuntimeError):
        x = frame.residues[nresidues]
    with pytest.raises(RuntimeError):
        x = frame.residues[-nresidues - 1]

    with pytest.raises(RuntimeError):
        x = frame.molecules[nchains]
    with pytest.raises(RuntimeError):
        x = frame.molecules[-nchains - 1]


def test_tracking_atom_refernces():
    frame = make_polyglycine([("A", 1)])
    last_atom = frame.atoms[-1]  # store reference to Atom in python variable
    frame = None  # release the reference to Frame and cause cascade deletion of everything
    with pytest.raises(Exception):
        last_atom.name  # access to destroyed elements is prohibited, exception raised


def test_tracking_residue_refernces():
    frame = make_polyglycine([("A", 1)])
    last_residue = frame.residues[-1]  # store reference to Atom in python variable
    frame = None  # release the reference to Frame and cause cascade deletion of everything
    with pytest.raises(Exception):
        last_residue.name  # access to destroyed elements is prohibited, exception raised


def test_tracking_chain_refernces():
    frame = make_polyglycine([("A", 1)])
    last_chain = frame.molecules[-1]  # store reference to Atom in python variable
    frame = None  # release the reference to Frame and cause cascade deletion of everything
    with pytest.raises(Exception):
        last_chain.name  # access to destroyed elements is prohibited, exception raised

def test_operators():
    frame = make_polyglycine([("A", 2)])

    a1 = frame.residues[0].atoms
    a2 = frame.residues[-1].atoms

    total = frame.atoms.size

    assert total == a1.size + a2.size

    assert (a1 - a2).size == a1.size
    assert (a2 - a1).size == a2.size
    assert (a1 + a2).size == total
    assert (a2 + a1).size == total
    assert (a1 * a2).size == 0
    assert (a2 * a1).size == 0

    a1 -= a2
    assert a1.size == a2.size



def test_lookup_by_name():
    from pyxmolpp2.polymer import ChainName, ResidueId, AtomName, \
        OutOfRangeResidue, OutOfRangeFrame, OutOfRangeChain
    frame = make_polyglycine([("A",2)])

    frame[ChainName("A")]  # does not throw
    frame[ChainName("A")][ResidueId(2)] # does not throw
    frame[ChainName("A")][ResidueId(2)][AtomName("CA")] # does not throw
    frame[ChainName("A")][ResidueId(2)][AtomName("N")] # does not throw

    with pytest.raises(OutOfRangeFrame):
        frame[ChainName("B")] # does throw

    with pytest.raises(OutOfRangeChain):
        frame[ChainName("A")][ResidueId(3)] # does throw

    with pytest.raises(OutOfRangeResidue):
        frame[ChainName("A")][ResidueId(2)][AtomName("CX")] # does not throw





def test_lookup_after_rename():
    from pyxmolpp2.polymer import ChainName, ResidueId, AtomName, \
        OutOfRangeResidue, OutOfRangeFrame, OutOfRangeChain
    frame = make_polyglycine([("A",2)])

    frame[ChainName("A")]  # does not throw
    frame[ChainName("A")][ResidueId(2)] # does not throw
    frame[ChainName("A")][ResidueId(2)][AtomName("CA")] # does not throw

    frame[ChainName("A")][ResidueId(2)][AtomName("CA")].name = AtomName("CX")
    frame[ChainName("A")][ResidueId(2)].id = ResidueId(99)
    frame[ChainName("A")].name = ChainName("X")

    frame[ChainName("X")]  # does not throw
    frame[ChainName("X")][ResidueId(99)] # does not throw
    frame[ChainName("X")][ResidueId(99)][AtomName("CX")] # does not throw


def test_dumb_copy_lookup():
    from pyxmolpp2.polymer import Frame
    frame = make_polyglycine([("A", 3), ("B", 3)])

    frame2 = Frame(2)
    for c in frame.molecules:
        frame2.emplace(c)

    assert frame.molecules.size == frame2.molecules.size
    assert frame.residues.size == frame2.residues.size
    assert frame.atoms.size == frame2.atoms.size

    for x in frame2.atoms:
        assert x.residue[x.name] == x

    for x in frame2.residues:
        assert x.molecule[x.id] == x

    for x in frame2.molecules:
        assert x.frame[x.name] == x
        assert x.frame == frame2


def test_selection_strides():
    frame = make_polyglycine([("A", 2)])

    assert frame.atoms.size == 2 * 7
    assert frame.atoms[::2].size == 7
    assert frame.atoms[::-2].size == 7
    assert frame.atoms[:10].size == 10
    assert frame.atoms[:-10].size == 4
    assert frame.atoms[-10:].size == 10


def test_frame_copy():
    frame = make_polyglycine([("A", 2)])
    frame2 = frame.copy()

    frame.atoms[0].r.x = 1
    frame2.atoms[0].r.x = 2

    assert frame.atoms[0].r.x != frame2.atoms[0].r.x


def test_AtomSelection_transformations():
    from pyxmolpp2.geometry import Translation3d, XYZ
    frame = make_polyglycine([("A", 20)])

    ats = frame.atoms
    for a in ats:
        assert (a.r - XYZ(1,2,3)).len() == 0

    transformation = Translation3d(XYZ(1,2,3))
    ats.transform(transformation)
    for a in ats:
        assert (a.r - XYZ(2,4,6)).len() == 0

def test_Atom_setters():
    from pyxmolpp2.polymer import AtomName
    from pyxmolpp2.geometry import XYZ
    frame = make_polyglycine([("A", 1)])
    a = frame.atoms[0]

    a.name = AtomName("X")
    assert a.name == AtomName("X")

    a.aName = AtomName("Y")
    assert a.aName == AtomName("Y")

    a.id = 5
    assert a.id == 5

    a.aId = 7
    assert a.aId == 7

    a.r = XYZ(9,9,9)
    assert (a.r - XYZ(9,9,9)).len() == 0

def test_Residue_setters():
    from pyxmolpp2.polymer import ResidueName, ResidueId
    frame = make_polyglycine([("A", 1)])
    r = frame.residues[0]

    r.name = ResidueName("X")
    assert r.name == ResidueName("X")

    r.rName = ResidueName("Y")
    assert r.rName == ResidueName("Y")

    r.id = ResidueId(5)
    assert r.id == ResidueId(5)

    r.rId = ResidueId(7)
    assert r.rId == ResidueId(7)

def test_Chain_setters():
    from pyxmolpp2.polymer import ChainName
    frame = make_polyglycine([("A", 1)])
    c = frame.molecules[0]

    c.name = ChainName("X")
    assert c.name == ChainName("X")

    c.cName = ChainName("Y")
    assert c.cName == ChainName("Y")


def test_AtomSelection_construction_from_list():
    from pyxmolpp2.polymer import AtomSelection

    frame = make_polyglycine([("A", 20)])
    atom_list = [a for a in frame.atoms]

    asel = AtomSelection(atom_list)

    assert asel == frame.atoms

def test_ResidueSelection_construction_from_list():
    from pyxmolpp2.polymer import ResidueSelection

    frame = make_polyglycine([("A", 20)])
    thelist = [a for a in frame.residues]

    sel = ResidueSelection(thelist)

    assert sel == frame.residues


def test_bad_selection_construction_from_list():
    from pyxmolpp2.polymer import AtomSelection, ChainSelection

    frame = make_polyglycine([("A", 20)])

    with pytest.raises(Exception):
        AtomSelection([a for a in frame.molecules])
    with pytest.raises(Exception):
        ChainSelection([a for a in frame.residues])


def test_frame_buf_output():
    from pyxmolpp2.pdb import StandardPdbRecords
    from io import StringIO
    frame = make_polyglycine([("A", 20)])
    output = StringIO()
    frame.to_pdb(output)
    assert output.getvalue().splitlines()[-1].strip() == "TER"

    output = StringIO()
    frame.to_pdb(output, StandardPdbRecords.instance())
    assert output.getvalue().splitlines()[-1].strip() == "TER"


def test_frame_file_output():
    frame = make_polyglycine([("A", 20)])

    output = "temp.pdb"
    frame.to_pdb(output)
    with open("temp.pdb") as output:
        assert output.readlines()[-1].strip() == "TER"

    os.unlink("temp.pdb")

    output = "temp.pdb"
    frame.to_pdb(output)

    with open("temp.pdb") as output:
        assert output.readlines()[-1].strip() == "TER"
    os.unlink("temp.pdb")


def test_anything_to_pdb_file():
    from pyxmolpp2.pdb import StandardPdbRecords
    frame = make_polyglycine([("A", 20)])

    output = "temp.pdb"
    frame.to_pdb(output)

    frame.to_pdb(output, StandardPdbRecords.instance())
    frame.atoms.to_pdb(output)
    frame.molecules.to_pdb(output)
    frame.residues.to_pdb(output)
    frame.atoms[0].to_pdb(output)
    frame.molecules[0].to_pdb(output)
    frame.residues[0].to_pdb(output)

    frame.to_pdb(output, StandardPdbRecords.instance())
    frame.atoms.to_pdb(output, StandardPdbRecords.instance())
    frame.molecules.to_pdb(output, StandardPdbRecords.instance())
    frame.residues.to_pdb(output, StandardPdbRecords.instance())
    frame.atoms[0].to_pdb(output, StandardPdbRecords.instance())
    frame.molecules[0].to_pdb(output, StandardPdbRecords.instance())
    frame.residues[0].to_pdb(output, StandardPdbRecords.instance())
    os.unlink("temp.pdb")


def test_anything_to_pdb_buffer():
    from pyxmolpp2.pdb import StandardPdbRecords
    from io import StringIO
    frame = make_polyglycine([("A", 20)])

    with StringIO() as output:
        frame.to_pdb(output)

        frame.to_pdb(output, StandardPdbRecords.instance())
        frame.atoms.to_pdb(output)
        frame.molecules.to_pdb(output)
        frame.residues.to_pdb(output)
        frame.atoms[0].to_pdb(output)
        frame.molecules[0].to_pdb(output)
        frame.residues[0].to_pdb(output)

        frame.to_pdb(output, StandardPdbRecords.instance())
        frame.atoms.to_pdb(output, StandardPdbRecords.instance())
        frame.molecules.to_pdb(output, StandardPdbRecords.instance())
        frame.residues.to_pdb(output, StandardPdbRecords.instance())
        frame.atoms[0].to_pdb(output, StandardPdbRecords.instance())
        frame.molecules[0].to_pdb(output, StandardPdbRecords.instance())
        frame.residues[0].to_pdb(output, StandardPdbRecords.instance())


def test_frame_buf_exceptions():
    frame = make_polyglycine([("A", 20)])

    with pytest.raises(TypeError):
        frame.to_pdb([])

    with pytest.raises(TypeError):
        frame.to_pdb({})
