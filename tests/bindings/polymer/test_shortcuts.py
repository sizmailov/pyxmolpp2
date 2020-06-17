import pytest
from make_polygly import make_polyglycine


def test_shorthands():
    import numpy as np
    from pyxmolpp2 import Rotation, Translation, XYZ, Degrees, Frame, calc_alignment

    frame = make_polyglycine([("A", 20)])
    for a in frame.atoms:
        a.r = XYZ(*np.random.random(3))
    frame2 = Frame(frame)

    assert frame2 != frame
    print(frame, frame2)
    asel = frame.atoms
    asel2 = frame2.atoms

    asel.coords.mean()
    # asel.mass_center([1.0] * asel.size) # todo: enable
    # asel.inertia_tensor([1.0] * asel.size) # todo: enable
    asel.coords.inertia_tensor()

    T = Translation(XYZ(1, 0, 0))
    asel2.coords.apply(T)
    print(asel.coords.rmsd(asel2.coords))
    assert np.isclose(asel.coords.rmsd(asel2.coords), 1.0)
    assert np.isclose(asel.coords.rmsd(asel2.coords), 1.0)
    asel2.coords.apply(T.inverted())
    assert np.isclose(asel.coords.rmsd(asel2.coords), 0.0)

    T = Translation(XYZ(1, 0, 0)) * Rotation(XYZ(1, 1, 1), Degrees(45))
    asel.coords.apply(asel.coords.alignment_to(asel2.coords))
    assert np.isclose(asel.coords.rmsd(asel2.coords), 0)

    asel2.coords.apply(T)
    asel.coords.apply(asel.alignment_to(asel2))  # todo: replace with .align_to()
    assert np.isclose(asel.coords.rmsd(asel2.coords), 0)

    asel2.coords.apply(T)
    asel2.guess_mass()
    asel.guess_mass()
    asel.coords.apply(asel.alignment_to(asel2, weighted=True))  # todo: replace with .align_to()
    assert np.isclose(asel.rmsd(asel2), 0)
    assert np.isclose(asel.rmsd(asel2, weighted=True), 0)

    T = Translation(XYZ(1, 0, 0)) * Rotation(XYZ(1, 1, 1), Degrees(45))
    asel2.coords.apply(T)

    assert np.allclose(T.matrix3d(), calc_alignment(asel2.coords.values, asel.coords.values).matrix3d())
    assert np.allclose(T.matrix3d(), asel.alignment_to(asel2).matrix3d())
