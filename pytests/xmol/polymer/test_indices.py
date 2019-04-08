def make_polyglycine(chain_lengths, no_reserve=True):
    from pyxmolpp2.polymer import Frame
    from pyxmolpp2.polymer import ChainName
    from pyxmolpp2.polymer import AtomName
    from pyxmolpp2.polymer import ResidueName, ResidueId
    from pyxmolpp2.geometry import XYZ

    aid = 1
    rid = 1
    frame = Frame(0)
    for chainId, N in chain_lengths:
        if no_reserve:
            c = frame.emplace(ChainName(chainId))
        else:
            c = frame.emplace(ChainName(chainId), N)
        for i in range(N):
            if no_reserve:
                r = c.emplace(ResidueName("GLY"), ResidueId(rid))
            else:
                r = c.emplace(ResidueName("GLY"), ResidueId(rid), 7)

            rid += 1
            for aname in ["N", "H", "CA", "HA2", "HA3", "C", "O"]:
                r.emplace(AtomName(aname), aid, XYZ(1, 2, 3))
                aid += 1

    return frame


def test_selection_bool_indexing():
    import numpy as np
    frame = make_polyglycine([("A", 20)])
    asel = frame.asAtoms

    ind = np.array([a.name.str == "C" for a in asel])

    subset = asel[ind]

    assert subset.size == 20


def test_selection_int_indexing():
    import numpy as np
    from pyxmolpp2.polymer import aName
    frame = make_polyglycine([("A", 20)])
    asel = frame.asAtoms

    ind = np.array([i for i, a in enumerate(asel) if a.name.str == "C"])
    ind2 = asel.index( aName == "C")
    subset = asel[ind]
    assert np.allclose(ind, ind2)
    assert subset.size == 20


test_selection_bool_indexing()
test_selection_int_indexing()
