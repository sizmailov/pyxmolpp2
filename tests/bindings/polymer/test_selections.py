import pytest
import os


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

def test_dead_selection_access():
    from pyxmolpp2.polymer import AtomSelection, ChainSelection, DeadResidueSelectionAccess, DeadChainSelectionAccess, DeadAtomSelectionAccess
    from pyxmolpp2.geometry import UniformScale3d

    frame = make_polyglycine([("A", 20)])
    asel = frame.asAtoms
    rsel = frame.asResidues
    csel = frame.asChains

    del frame

    with pytest.raises(DeadAtomSelectionAccess):
        asel[0]

    with pytest.raises(DeadAtomSelectionAccess):
        asel[0:-1]

    with pytest.raises(RuntimeError):
        asel.filter(lambda a: True)

    with pytest.raises(DeadAtomSelectionAccess):
        asel.toCoords

    with pytest.raises(DeadAtomSelectionAccess):
        asel.geom_center()

    with pytest.raises(DeadAtomSelectionAccess):
        asel.transform(UniformScale3d(1))

    with pytest.raises(DeadAtomSelectionAccess):
        asel.asResidues

    with pytest.raises(DeadAtomSelectionAccess):
        asel.asChains

    del asel

    with pytest.raises(DeadResidueSelectionAccess):
        rsel[0]

    with pytest.raises(DeadResidueSelectionAccess):
        rsel[0:-1]

    with pytest.raises(RuntimeError):
        rsel.filter(lambda a: True)

    with pytest.raises(DeadResidueSelectionAccess):
        rsel.asAtoms

    with pytest.raises(DeadResidueSelectionAccess):
        rsel.asChains

    del rsel

    with pytest.raises(DeadChainSelectionAccess):
        csel[0]

    with pytest.raises(DeadChainSelectionAccess):
        csel[0:-1]

    with pytest.raises(RuntimeError):
        csel.filter(lambda a: True)

    with pytest.raises(DeadChainSelectionAccess):
        csel.asAtoms

    with pytest.raises(DeadChainSelectionAccess):
        csel.asResidues
