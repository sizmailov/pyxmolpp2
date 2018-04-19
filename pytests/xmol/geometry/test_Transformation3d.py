import pytest
import math


def test_composition():
    from pyxmolpp2.geometry import calc_alignment, XYZ, VectorXYZ, calc_rmsd, Rotation3d, Translation3d, Degrees, UniformScale3d

    R = Rotation3d(XYZ(1,1,1),Degrees(39))
    T = Translation3d(XYZ(7,1,2))
    S = UniformScale3d(1.5)
    G = R*T*S*R*T

    def check(G):
        r = XYZ(5,6,4)
        assert ((G*G.inverted()).transform(r)-r).len() == pytest.approx(0)

    check(R)
    check(T)
    check(S)
    check(G)

    for a in [R,T,S,G]:
        for b in [R,T,S,G]:
            check(a*b)


