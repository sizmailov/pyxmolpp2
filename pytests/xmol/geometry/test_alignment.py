import pytest
import math

def test_calc_rmsd():
    from pyxmolpp2.geometry import calc_alignment, XYZ, VectorXYZ, calc_rmsd

    a = VectorXYZ([XYZ(1,2,3)])
    b = VectorXYZ([XYZ(3,4,5)])

    assert calc_rmsd(a,b) == pytest.approx( math.sqrt(2**2*3) )

    a = VectorXYZ([XYZ(1,2,3),XYZ(1,2,3)])
    b = VectorXYZ([XYZ(3,4,5),XYZ(1,2,3)])

    assert calc_rmsd(a,b) == pytest.approx( math.sqrt(2**2*3*0.5) )

def test_calc_rmsd_exception():
    from pyxmolpp2.geometry import calc_alignment, XYZ, VectorXYZ, calc_rmsd, GeometryException, Transformation3d

    a = VectorXYZ([])
    with pytest.raises(GeometryException):
        calc_rmsd(a, a)

    with pytest.raises(GeometryException):
        calc_rmsd(a, a, Transformation3d())

    a = [XYZ(1, 2, 3)] * 10

    with pytest.raises(GeometryException):
        calc_rmsd(VectorXYZ(a[:4]), VectorXYZ(a))

    with pytest.raises(GeometryException):
        calc_rmsd(VectorXYZ(a[:4]), VectorXYZ(a), Transformation3d())


def test_calc_geom_center_exception():
    from pyxmolpp2.geometry import calc_geom_center, XYZ, VectorXYZ, calc_rmsd, GeometryException, Transformation3d

    with pytest.raises(GeometryException):
        calc_geom_center(VectorXYZ([]))

    calc_geom_center(VectorXYZ([XYZ(1,2,3)]))



def test_alignment_exception():
    from pyxmolpp2.geometry import calc_alignment, XYZ, VectorXYZ, calc_rmsd, AlignmentError

    a = [XYZ(1, 2, 3)] * 10

    with pytest.raises(AlignmentError):
        calc_alignment(VectorXYZ(a[:2]), VectorXYZ(a[:2]))

    calc_alignment(VectorXYZ(a[:3]), VectorXYZ(a[:3]))

    with pytest.raises(AlignmentError):
        calc_alignment(VectorXYZ(a[:4]), VectorXYZ(a))



def test_calc_alignment():
    from pyxmolpp2.geometry import calc_alignment, XYZ, VectorXYZ, calc_rmsd, Rotation3d, Translation3d, Degrees

    a = VectorXYZ([XYZ(1,2,3),XYZ(1,2,5),XYZ(4,2,7),XYZ(8,1,4)])
    G = Rotation3d(XYZ(7,6,5),Degrees(12))*Translation3d(XYZ(8,-9,1))
    b = VectorXYZ([ G.transform(x) for x in a ])



    G2 = calc_alignment(a,b)
    assert calc_rmsd(a,b) > 1
    assert calc_rmsd(a,b,G2) == pytest.approx(0)



def test_calc_inertia_tensor():
    from pyxmolpp2.geometry import calc_inertia_tensor, XYZ, VectorXYZ, calc_rmsd, Rotation3d, Translation3d, Degrees
    import numpy as np

    a = VectorXYZ([XYZ(0,1,0),XYZ(1,0,0),XYZ(-1,0,0),XYZ(0,-1,0)])
    x = calc_inertia_tensor(a)

    assert np.allclose(x,np.diag([2,2,4]))

    m = [10,1,1,10]
    x = calc_inertia_tensor(a ,m)
    assert np.allclose(x,np.diag([20,2,22]))


def test_calc_inertia_tensor_off_diagonal():
    from pyxmolpp2.geometry import calc_inertia_tensor, XYZ, VectorXYZ, calc_rmsd, Rotation3d, Translation3d, Degrees
    import numpy as np

    N = 1000
    a = VectorXYZ.from_numpy(np.random.random((N, 3)))
    x = calc_inertia_tensor(a)

    V, _, _ = np.linalg.svd(x, full_matrices=True)
    a.transform(Rotation3d(V.T))
    I = calc_inertia_tensor(a)

    assert np.allclose([I[0, 1], I[0, 2], I[1, 0], I[1, 2], I[2, 0], I[2, 1]], 0)


def test_calc_mass_center():
    from pyxmolpp2.geometry import calc_mass_center, XYZ, VectorXYZ, calc_rmsd, Rotation3d, Translation3d, Degrees
    import numpy as np

    a = VectorXYZ([XYZ(0,1,0),XYZ(1,0,0),XYZ(-1,0,0),XYZ(0,-1,0)])

    m = np.random.random((len(a),)).tolist()
    x = calc_mass_center(a, m)
    cm = sum([ r*M for r,M in zip(a,m) ], XYZ(0,0,0))/np.sum(m)
    assert (x-cm).len() == pytest.approx(0)





