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


def test_transformation_3d():
    import numpy as np
    from pyxmolpp2.geometry import calc_alignment, XYZ, VectorXYZ, calc_rmsd, Rotation3d, Translation3d, Degrees, UniformScale3d

    R = Rotation3d(XYZ(1,0,0), Degrees(45))
    T = Translation3d(XYZ(1,2,5))

    G = T*R

    m = G.matrix3d()
    v = G.vector3d()

    assert pytest.approx(m[0,0]) == 1
    assert pytest.approx(m[0,1]) == 0
    assert pytest.approx(m[0,2]) == 0
    assert pytest.approx(np.sqrt(2)/2) == m[2,1]

    assert v.x == 1
    assert v.y == 2
    assert v.z == 5


def test_rotation_decomposition():
    import numpy as np
    from pyxmolpp2.geometry import XYZ, Rotation3d,  Degrees

    for ax, theta in [
        (XYZ(0, 0, 1), Degrees(30)),
        (XYZ(0, 0, 1), Degrees(70)),
        (XYZ(0, 1, 0), Degrees(70)),
        (XYZ(1, 0, 0), Degrees(70)),
        (XYZ(1, 1, 0), Degrees(70)),
        (XYZ(1, 1, 1), Degrees(70)),
        (XYZ(1, 0, 1), Degrees(0)),
        (XYZ(1, 1, 1), Degrees(0)),
    ]:
        R = Rotation3d(ax, theta)

        ax1, theta1 = R.axis(), R.theta()

        R2 = Rotation3d(ax1, theta1)

        assert np.allclose(R.matrix3d(),R2.matrix3d())
