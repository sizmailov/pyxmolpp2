import pytest
import os

def test_Frame():
    from pyxmolpp2.polymer import Frame
    from pyxmolpp2.polymer import ChainName
    from pyxmolpp2.polymer import AtomName
    from pyxmolpp2.polymer import ResidueName, ResidueId
    from pyxmolpp2.geometry import XYZ

    f = Frame(5)
    c = f.emplace(ChainName("A"), 1)
    r = c.emplace(ResidueName("LYS"), ResidueId(1))
    a = r.emplace(AtomName("CA"), 1, XYZ(1,2,3))

    assert a.residue == r
    assert a.chain == c
    assert a.frame == f

    assert r.asAtoms[0] == a
    assert c.asAtoms[0] == a
    assert f.asAtoms[0] == a

    assert r.asAtoms.asResidues[0] == r
    assert c.asAtoms.asResidues[0] == r
    assert f.asAtoms.asResidues[0] == r

    assert r.asAtoms.asResidues.asChains[0] == c
    assert c.asAtoms.asResidues.asChains[0] == c
    assert f.asAtoms.asResidues.asChains[0] == c


    assert r.chain == c
    assert r.frame == f

    assert c.frame == f

    assert c.asResidues[0] == r
    assert f.asResidues[0] == r

    assert f.asChains[0] == c

def make_polyglycine( chain_lengths, no_reserve=True):
    from pyxmolpp2.polymer import Frame
    from pyxmolpp2.polymer import ChainName
    from pyxmolpp2.polymer import AtomName
    from pyxmolpp2.polymer import ResidueName, ResidueId
    from pyxmolpp2.geometry import XYZ

    aid=1
    rid=1
    frame = Frame(0)
    for chainId, N in chain_lengths:
        if no_reserve:
            c = frame.emplace(ChainName(chainId))
        else:
            c = frame.emplace(ChainName(chainId),N)
        for i in range(N):
            if no_reserve:
                r = c.emplace(ResidueName("GLY"),ResidueId(rid))
            else:
                r = c.emplace(ResidueName("GLY"),ResidueId(rid),7)

            rid+=1
            for aname in ["N","H","CA","HA2","HA3","C","O"]:
                r.emplace(AtomName(aname),aid,XYZ(1,2,3))
                aid+=1

    return frame


@pytest.mark.skipif("PYXMOLPP_RUN_PERFORMANCE_TESTS" not in os.environ,
                    reason="Run performance tests only in release mode")
def test_big_construction():
    from timeit import default_timer as timer


    for no_reserve in [True, False]:
        for n in [10,100,1000,2000]:
            start = timer()
            frame = make_polyglycine([("A", n)], no_reserve=no_reserve)
            end = timer()
            assert frame.asAtoms.size == n*7
            #print("%5d  %.5f"%(n,end-start))

def test_filters():
    n = 100
    frame = make_polyglycine([("A", n)])

    # atom filters
    assert frame.asAtoms.filter(lambda a: a.chain.name.str == "A" ).size == n*7
    assert frame.asAtoms.filter(lambda a: a.name.str == "CA" ).size == n
    assert frame.asAtoms.filter(lambda a: a.name.str == "N" ).size == n
    assert frame.asAtoms.filter(lambda a: a.name.str == "XXXX" ).size == 0

    # residue filters

    assert frame.asResidues.filter(lambda r: r.name.str == "GLY").size == n
    assert frame.asResidues.filter(lambda r: r.name.str == "LYS").size == 0
    assert frame.asResidues.filter(lambda r: r.id == 1).size == 1
    assert frame.asResidues.filter(lambda r: r.size == 1).size == 0

    # chain filters
    assert frame.asChains.filter(lambda c: c.name.str == "1" ).size == 0
    assert frame.asChains.filter(lambda c: c.name.str == "A" ).size == 1
    assert frame.asChains.filter(lambda c: c.size > 2).size == 1

def test_iterable():

    frame = make_polyglycine([("A",10)])

    assert len(list(frame.asChains)) == frame.asChains.size
    assert len(list(frame.asResidues)) == frame.asResidues.size
    assert len(list(frame.asAtoms)) == frame.asAtoms.size

def test_repr():

    frame = make_polyglycine([("A",10)])

    str(frame)
    str(frame.asChains[0].name)
    str(frame.asResidues[0].name)
    str(frame.asAtoms[0].name)
    str([frame.asChains[0].name,
            frame.asResidues[0].name,
            frame.asAtoms[0].name]
    )

    str(frame.asChains[0])
    str(frame.asResidues[0])
    str(frame.asAtoms[0])

    str(frame.asChains)
    str(frame.asResidues)
    str(frame.asAtoms)


def test_for_each():

    frame = make_polyglycine([("A",10)])

    count = [0]
    def inc_count(_):
        count[0] += 1

    count[0] = 0; frame.asChains.for_each(inc_count)
    assert frame.asChains.size == count[0];

    count[0] = 0; frame.asResidues.for_each(inc_count)
    assert frame.asResidues.size == count[0];

    count[0] = 0; frame.asAtoms.for_each(inc_count)
    assert frame.asAtoms.size == count[0];



def test_assignment():
    from pyxmolpp2.geometry import XYZ
    frame = make_polyglycine([("A",1)])
    for a in frame.asAtoms:
        #print(a,a.r)
        pass

    def translate_by_dr(a):
        #print(a,a.r)
        a.r = a.r+XYZ(1,2,3)

    frame.asAtoms.for_each(translate_by_dr)

    assert frame.asAtoms[0].r.x == 1*2
    assert frame.asAtoms[0].r.y == 2*2
    assert frame.asAtoms[0].r.z == 3*2


def test_selection_exceptions():
    from pyxmolpp2.polymer import DeadAtomSelectionAccess, DeadResidueSelectionAccess, DeadChainSelectionAccess

    zombie_selection = make_polyglycine([("A",1)]).asAtoms
    with pytest.raises(DeadAtomSelectionAccess):
        for a in zombie_selection:
            pass

    zombie_selection = make_polyglycine([("A",1)]).asResidues
    with pytest.raises(DeadResidueSelectionAccess):
        for a in zombie_selection:
            pass

    zombie_selection = make_polyglycine([("A",1)]).asChains
    with pytest.raises(DeadChainSelectionAccess):
        for a in zombie_selection:
            pass

def test_deleted_element_access_exceptions():
    from pyxmolpp2.polymer import DeletedAtomAccess, DeletedChainAccess, DeletedResidueAccess

    frame = make_polyglycine([("A",1)])
    sel = frame.asAtoms
    sel[0].delete()
    with pytest.raises(DeletedAtomAccess):
        for a in sel:
            pass

    frame = make_polyglycine([("A",2)])
    sel = frame.asResidues
    sel[0].delete()
    with pytest.raises(DeletedResidueAccess):
        for a in sel:
            pass

    frame = make_polyglycine([("A",2)]*2)
    sel = frame.asChains
    sel[0].delete()
    with pytest.raises(DeletedChainAccess):
        for a in sel:
            pass



def test_range_exceptions():
    from pyxmolpp2.polymer import OutOfRangeAtomSelection,\
        OutOfRangeChainSelection,\
        OutOfRangeResidueSelection, \
        OutOfRangeChain, \
        OutOfRangeFrame, \
        OutOfRangeResidue, \
        AtomName


    frame = make_polyglycine([("A",1),("B",4),("C",3)])

    # with pytest.raises(OutOfRangeFrame):
    #     x = frame[3]

    # with pytest.raises(OutOfRangeChain):
    #     x = frame.asChains[0][3]

    with pytest.raises(OutOfRangeResidue):
        x = frame.asResidues[0][AtomName("ca")]

    assert frame.asResidues[0][AtomName("CA")].name.str == "CA"

    natoms = frame.asAtoms.size
    nresidues = frame.asResidues.size
    nchains = frame.asChains.size

    frame.asAtoms[0]
    frame.asAtoms[-natoms]
    frame.asResidues[0]
    frame.asResidues[-nresidues]
    frame.asChains[0]
    frame.asChains[-nchains]

    with pytest.raises(OutOfRangeAtomSelection):
        x = frame.asAtoms[natoms]
    with pytest.raises(OutOfRangeAtomSelection):
        x = frame.asAtoms[-natoms-1]


    with pytest.raises(OutOfRangeResidueSelection):
        x = frame.asResidues[nresidues]
    with pytest.raises(OutOfRangeResidueSelection):
        x = frame.asResidues[-nresidues-1]

    with pytest.raises(OutOfRangeChainSelection):
        x = frame.asChains[nchains]
    with pytest.raises(OutOfRangeChainSelection):
        x = frame.asChains[-nchains-1]


def test_tracking_atom_refernces():

    frame = make_polyglycine([("A",1)])
    last_atom = frame.asAtoms[-1]  # store reference to Atom in python variable
    frame = None                   # release the reference to Frame and cause cascade deletion of everything
    with pytest.raises(Exception):
        last_atom.name             # access to destroyed elements is prohibited, exception raised


def test_tracking_residue_refernces():

    frame = make_polyglycine([("A",1)])
    last_residue = frame.asResidues[-1]  # store reference to Atom in python variable
    frame = None                   # release the reference to Frame and cause cascade deletion of everything
    with pytest.raises(Exception):
        last_residue.name             # access to destroyed elements is prohibited, exception raised

def test_tracking_chain_refernces():
    frame = make_polyglycine([("A",1)])
    last_chain = frame.asChains[-1]  # store reference to Atom in python variable
    frame = None                   # release the reference to Frame and cause cascade deletion of everything
    with pytest.raises(Exception):
        last_chain.name             # access to destroyed elements is prohibited, exception raised

def test_operators():

    frame = make_polyglycine([("A",2)])

    a1 = frame.asResidues[0].asAtoms
    a2 = frame.asResidues[-1].asAtoms

    total = frame.asAtoms.size

    assert total == a1.size+a2.size

    assert (a1-a2).size == a1.size
    assert (a2-a1).size == a2.size
    assert (a1+a2).size == total
    assert (a2+a1).size == total
    assert (a1*a2).size == 0
    assert (a2*a1).size == 0

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
    frame = make_polyglycine([("A", 3),("B", 3)])

    frame2 = Frame(2)
    for c in frame.asChains:
        frame2.emplace(c)

    assert frame.asChains.size == frame2.asChains.size
    assert frame.asResidues.size == frame2.asResidues.size
    assert frame.asAtoms.size == frame2.asAtoms.size

    for x in frame2.asAtoms:
        assert x.residue[x.name] == x

    for x in frame2.asResidues:
        assert x.chain[x.id] == x

    for x in frame2.asChains:
        assert x.frame[x.name] == x
        assert x.frame == frame2


def test_selection_strides():

    frame = make_polyglycine([("A", 2)])

    assert frame.asAtoms.size == 2*7
    assert frame.asAtoms[::2].size == 7
    assert frame.asAtoms[::-2].size == 7
    assert frame.asAtoms[:10].size == 10
    assert frame.asAtoms[:-10].size == 4
    assert frame.asAtoms[-10:].size == 10

def test_AtomName_compare():
    from pyxmolpp2.polymer import AtomName
    frame = make_polyglycine([("A", 1)])
    a = frame.asAtoms[0]
    assert a.name == AtomName(a.name.str)


def test_frame_copy():

    frame = make_polyglycine([("A", 2)])
    frame2 = frame.copy()

    frame.asAtoms[0].r.x = 1
    frame2.asAtoms[0].r.x = 2

    assert frame.asAtoms[0].r.x != frame2.asAtoms[0].r.x


def test_AtomSelection_transformations():
    from pyxmolpp2.geometry import Translation3d, XYZ
    frame = make_polyglycine([("A", 20)])

    ats = frame.asAtoms
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
    a = frame.asAtoms[0]

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
    r = frame.asResidues[0]

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
    c = frame.asChains[0]

    c.name = ChainName("X")
    assert c.name == ChainName("X")

    c.cName = ChainName("Y")
    assert c.cName == ChainName("Y")

