import pytest
import math

def test_distance():
    from pyxmolpp2.geometry import XYZ, distance
    a = XYZ(1,2,3)
    b = XYZ(4,5,6)

    assert distance(a,b) == pytest.approx(math.sqrt((3**2 * 3)))


def test_distance2():
    from pyxmolpp2.geometry import XYZ, distance2
    a = XYZ(1,2,3)
    b = XYZ(4,5,6)

    assert distance2(a,b) == pytest.approx((3**2 * 3))


def test_angle():
    from pyxmolpp2.geometry import XYZ, angle
    a = XYZ(0,0,1)
    b = XYZ(0,0,0)
    c = XYZ(1,0,0)

    assert angle(a,b,c).degrees == pytest.approx(90)

    a = XYZ(0,0,1)
    b = XYZ(0,0,0)
    c = XYZ(1,0,1)

    assert angle(a,b,c).degrees == pytest.approx(45)


def test_dihedral():
    from pyxmolpp2.geometry import XYZ, dihedral_angle
    a = XYZ(1,0,0)
    b = XYZ(0,0,0)
    c = XYZ(0,0,1)
    d = XYZ(0,1,1)

    assert dihedral_angle(a,b,c,d).degrees == pytest.approx(90)

    a = XYZ(1,0,0)
    b = XYZ(0,0,0)
    c = XYZ(0,0,1)
    d = XYZ(1,1,1)

    assert dihedral_angle(a,b,c,d).degrees == pytest.approx(45)
