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


def test_shorthands():
    import numpy as np

    from pyxmolpp2.geometry import Rotation3d, Translation3d, XYZ, Degrees

    frame = make_polyglycine([("A", 20)])
    frame2 = frame.copy()

    asel = frame.asAtoms
    asel2 = frame2.asAtoms

    asel.geom_center()
    asel.mass_center([1.0] * asel.size)
    asel.inertia_tensor([1.0] * asel.size)
    asel.geom_inertia_tensor()

    T = Translation3d(XYZ(1, 0, 0))
    asel2.transform(T)

    # print(list(asel.toCoords))
    # print(list(asel2.toCoords))

    assert np.isclose(asel.rmsd(asel2), 1.0)
    assert np.isclose(asel.rmsd(asel2.toCoords), 1.0)
    assert np.isclose(asel.rmsd(asel2.toCoords.transform(T.inverted())), 0.0)

    T = Translation3d(XYZ(1, 0, 0)) * Rotation3d(XYZ(1, 1, 1), Degrees(45))
    asel.align_to(asel2)
    assert np.isclose(asel.rmsd(asel2), 0)

    asel2.transform(T)
    asel.align_to(asel2.toCoords)
    assert np.isclose(asel.rmsd(asel2), 0)
