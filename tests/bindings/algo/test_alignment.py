import pytest
import math
import numpy as np


def test_calc_rmsd():
    from pyxmolpp2 import calc_alignment, XYZ, calc_rmsd

    a = np.array([(1, 2, 3)])
    b = np.array([(3, 4, 5)])

    assert calc_rmsd(a, b) == pytest.approx(2 * math.sqrt(3))

    a = np.array([(1, 2, 3), (1, 2, 3)])
    b = np.array([(3, 4, 5), (1, 2, 3)])

    assert calc_rmsd(a, b) == pytest.approx(2 * math.sqrt(3 * 0.5))


def test_calc_rmsd_exception():
    from pyxmolpp2 import calc_alignment, XYZ, calc_rmsd, Transformation

    a = np.zeros(0)
    with pytest.raises(TypeError):
        calc_rmsd(a, a)

    a = np.array([[1, 2, 3]] * 10)

    with pytest.raises(TypeError):
        calc_rmsd(a[:4].T, a.T)

    assert pytest.approx(0) == calc_rmsd(a[:4], a[:4])


def test_alignment_exception():
    from pyxmolpp2 import calc_alignment, GeomError

    a = np.array([(1, 2, 3)] * 10)

    # two coords is not enough
    with pytest.raises(GeomError):
        calc_alignment(a[:2], a[:2])

    # three coords is just enough
    calc_alignment(a[:3], a[:3])

    # number of coords must match
    with pytest.raises(GeomError):
        calc_alignment(a[:4], a)


def test_calc_alignment():
    from pyxmolpp2 import calc_alignment, XYZ, calc_rmsd, Rotation, Translation, Degrees

    a = np.array([(1, 2, 3), (1, 2, 5), (4, 2, 7), (8, 1, 4)])
    G = Rotation(XYZ(7, 6, 5), Degrees(12)) * Translation(XYZ(8, -9, 1))
    b = np.array([G.transform(XYZ(*x)).values for x in a])

    G2 = calc_alignment(a, b)
    assert calc_rmsd(a, b) > 1
    c = np.array([G2.transform(XYZ(*x)).values for x in b])
    assert calc_rmsd(a, c) == pytest.approx(0)


def test_calc_inertia_tensor():
    from pyxmolpp2 import calc_inertia_tensor, XYZ, Rotation, Translation
    import numpy as np

    a = np.array([(0, 1, 0), (1, 0, 0), (-1, 0, 0), (0, -1, 0)])
    x = calc_inertia_tensor(a)

    assert np.allclose(x, np.diag([2, 2, 4]))


def test_calc_inertia_tensor_mass():
    from pyxmolpp2 import Frame, XYZ
    import numpy as np

    frame = Frame()
    mol = frame.add_molecule()
    residue = mol.add_residue()
    for r, m in zip([(0, 1, 0), (1, 0, 0), (-1, 0, 0), (0, -1, 0)], [10, 1, 1, 10]):
        atom = residue.add_atom()
        atom.mass = m
        atom.r = XYZ(*r)

    assert np.allclose(frame.coords.inertia_tensor(), np.diag([2, 2, 4]))
    assert np.allclose(frame.atoms.inertia_tensor(), np.diag([20, 2, 22]))


def _inertia_tensor(coords: np.ndarray):
    com = coords.mean(axis=0)

    xx = 0
    yy = 0
    zz = 0
    xy = 0
    xz = 0
    yz = 0

    for a in coords:
        x, y, z = a - com
        xx += y ** 2 + z ** 2
        yy += x ** 2 + z ** 2
        zz += y ** 2 + x ** 2
        xy += x * y
        xz += x * z
        yz += y * z

    return np.array([[xx, -xy, -xz],
                     [-xy, yy, -yz],
                     [-xz, -yz, zz]])


def test_calc_inertia_tensor_off_diagonal():
    from pyxmolpp2 import calc_inertia_tensor
    import numpy as np

    N = 1000
    a = np.random.random((N, 3))
    x = calc_inertia_tensor(a)

    assert np.allclose(x, _inertia_tensor(a))

    V, _, _ = np.linalg.svd(x, full_matrices=True)
    a = (V.T @ a.T).T
    I = calc_inertia_tensor(a)

    assert np.allclose([I[0, 1], I[0, 2], I[1, 0], I[1, 2], I[2, 0], I[2, 1]], 0)
