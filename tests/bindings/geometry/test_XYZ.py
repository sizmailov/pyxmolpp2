from pyxmolpp2 import XYZ


def test_construction():
    a = XYZ()
    b = XYZ(1, 2, 3)

    c = a + b

    assert c.x == 1
    assert c.y == 2
    assert c.z == 3


def test_operators():
    import numpy as np

    def as_np(xyz):
        return np.array([xyz.x, xyz.y, xyz.z])

    a = XYZ(5, 8, 1)
    a.x, a.y, a.z = 9, 8, 7

    assert a.x == 9
    assert a.y == 8
    assert a.z == 7

    b = XYZ(1, 2, 3)

    npa = as_np(a)
    npb = as_np(b)

    assert np.allclose(as_np(a + b), npa + npb)
    assert np.allclose(as_np(-a), -npa)
    assert np.allclose(as_np(a - b), npa - npb)
    assert np.allclose(as_np(a * 7), npa * 7)
    assert np.allclose(as_np(a / 7), npa / 7)
    assert np.allclose(as_np(7 * a), 7 * npa)
    assert np.allclose((a.dot(b)), np.dot(npa, npb))
    assert np.allclose(a.len(), np.linalg.norm(npa))
    assert np.allclose(a.len2(), np.linalg.norm(npa) ** 2)
    assert np.allclose(as_np(a.cross(b)), np.cross(npa, npb))


def test_conversion():
    import numpy as np

    def as_np(xyz):
        return np.array([xyz.x, xyz.y, xyz.z])

    a = XYZ(5, 8, 1)

    assert np.allclose(a.values, as_np(a))
