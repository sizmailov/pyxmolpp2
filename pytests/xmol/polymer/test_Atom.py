import pytest
import os

def test_Frame():
    from pyxmolpp2.polymer import Frame
    from pyxmolpp2.polymer import ChainName
    from pyxmolpp2.polymer import AtomName
    from pyxmolpp2.polymer import ResidueName
    from pyxmolpp2.geometry import XYZ

    f = Frame(5)
    c = f.emplace(ChainName("A"),1)
    r = c.emplace(ResidueName("LYS"),1)
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

def make_polyglycine( chain_lengths ):
    from pyxmolpp2.polymer import Frame
    from pyxmolpp2.polymer import ChainName
    from pyxmolpp2.polymer import AtomName
    from pyxmolpp2.polymer import ResidueName
    from pyxmolpp2.geometry import XYZ

    aid=1
    rid=1
    frame = Frame(0)
    for chainId, N in chain_lengths:
        c = frame.emplace(ChainName(chainId),N)
        for i in range(N):
            r = c.emplace(ResidueName("GLY"),rid,7)
            rid+=1
            for aname in ["N","H","CA","HA2","HA3","C","O"]:
                r.emplace(AtomName(aname),aid,XYZ(1,2,3))
                aid+=1

    return frame


@pytest.mark.skipif("PYXMOLPP_RUN_PERFORMANCE_TESTS" not in os.environ,
                    reason="Run performance tests only in release mode")
def test_big_construction():
    from timeit import default_timer as timer

    print()
    for n in [10,100,1000,2000]:
        start = timer()
        frame = make_polyglycine([("A", n)])
        end = timer()
        assert frame.asAtoms.size == n*7
        print "%5d  %.5f"%(n,end-start)

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

    print(frame)
    print(frame.asChains[0].name)
    print(frame.asResidues[0].name)
    print(frame.asAtoms[0].name)
    print([frame.asChains[0].name,
            frame.asResidues[0].name,
            frame.asAtoms[0].name]
    )

    print(frame.asChains[0])
    print(frame.asResidues[0])
    print(frame.asAtoms[0])

    print(frame.asChains)
    print(frame.asResidues)
    print(frame.asAtoms)


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
        print(a,a.r)

    def translate_by_dr(a):
        print(a,a.r)
        a.r = a.r+XYZ(1,2,3)

    frame.asAtoms.for_each(translate_by_dr)

    assert frame.asAtoms[0].r.x == 1*2
    assert frame.asAtoms[0].r.y == 2*2
    assert frame.asAtoms[0].r.z == 3*2


