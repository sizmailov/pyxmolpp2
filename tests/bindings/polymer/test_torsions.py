import pytest
from make_polygly import make_polyglycine


def test_torsional_angly_factory():
    import numpy as np
    from pyxmolpp2 import Rotation, Translation, XYZ, Degrees, Frame, calc_alignment, TorsionAngleFactory, TorsionAngle
    frame = make_polyglycine([('A', 10)])

    for res in frame.residues:
        omega = TorsionAngleFactory.omega(res)
        phi = TorsionAngleFactory.phi(res)
        psi = TorsionAngleFactory.psi(res)
        if res.id == 1:
            assert omega is None
            assert phi is None
        else:
            assert omega is not None
            assert phi is not None
        if res.id == 10:
            assert psi is None
        else:
            assert psi is not None

        assert TorsionAngleFactory.chi1(res) is None
        assert TorsionAngleFactory.chi2(res) is None
        assert TorsionAngleFactory.chi3(res) is None
        assert TorsionAngleFactory.chi4(res) is None
        assert TorsionAngleFactory.chi5(res) is None
