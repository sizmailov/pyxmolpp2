import pytest


def make_polyglycine(chain_lengths, no_reserve=True):
    from pyxmolpp2 import Frame
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
    rsel = frame.asResidues
    csel = frame.asChains

    ind = np.array([i for i, a in enumerate(asel) if a.name.str == "C"])
    ind2 = asel.index(aName == "C")
    subset = asel[ind]


    assert np.allclose(ind, ind2)
    assert subset.size == 20
    assert asel[ind2].size == 20

    # Bool array size doesn't match selection size
    with pytest.raises(RuntimeError):
        asel[np.array([True, False])]

    with pytest.raises(RuntimeError):
        rsel[np.array([True, False])]

    with pytest.raises(RuntimeError):
        csel[np.array([True, False])]

    # Bool array element type is not integer or bool
    with pytest.raises(RuntimeError):
        asel[np.array([1.0, 2.0])]

    with pytest.raises(RuntimeError):
        rsel[np.array([1.0, 2.0])]

    with pytest.raises(RuntimeError):
        csel[np.array([1.0, 2.0])]

    # Array dimension is not 1
    with pytest.raises(RuntimeError):
        asel[np.array([[0, 2], [1, 2]])]

    with pytest.raises(RuntimeError):
        rsel[np.array([[0, 2], [1, 2]])]

    with pytest.raises(RuntimeError):
        csel[np.array([[0, 2], [1, 2]])]

    assert frame.asResidues[np.array([0, 1])].size == 2
    assert frame.asChains[np.array([0])].size == 1  # array index -> selection
    assert frame.asChains[np.array([], dtype=int)].size == 0  # array index -> selection
    assert frame.asChains[0].size == 20  # int index -> Chain

if __name__ == "__main__":
    test_selection_bool_indexing()
    test_selection_int_indexing()
