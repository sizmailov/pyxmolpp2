import pytest
import os

def test_Frame():
    from pyxmolpp2.polymer import Frame
    from pyxmolpp2.polymer import ChainName
    from pyxmolpp2.polymer import AtomName
    from pyxmolpp2.polymer import ResidueName
    from pyxmolpp2.polymer import XYZ

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
    from pyxmolpp2.polymer import XYZ
    from tqdm import tqdm

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

