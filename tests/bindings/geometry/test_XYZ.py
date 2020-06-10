import pytest
import math
import numpy as np

from pyxmolpp2.geometry import XYZ, VectorXYZ, Translation3d, Rotation3d, UniformScale3d, Transformation3d, Degrees

def test_construction():

    a = XYZ()
    b = XYZ(1,2,3)

    c = a+b

    assert c.x == 1
    assert c.y == 2
    assert c.z == 3


def test_operators():
    import numpy as np

    def as_np(xyz):
        return np.array([xyz.x,xyz.y,xyz.z])

    a = XYZ(5,8,1)
    a.x,a.y,a.z = 9,8,7

    assert a.x == 9
    assert a.y == 8
    assert a.z == 7

    b = XYZ(1,2,3)

    npa = as_np(a)
    npb = as_np(b)

    assert np.allclose(as_np(a + b), npa + npb)
    assert np.allclose(as_np(-a), -npa )
    assert np.allclose(as_np(a - b), npa - npb)
    assert np.allclose(as_np(a * 7), npa*7)
    assert np.allclose(as_np(a / 7), npa/7)
    assert np.allclose(as_np(7*a ), 7*npa)
    assert np.allclose((a.dot(b)), np.dot(npa,npb))
    assert np.allclose(a.len(), np.linalg.norm(npa))
    assert np.allclose(a.len2(), np.linalg.norm(npa)**2)
    assert np.allclose(as_np(a.cross(b)), np.cross(npa,npb))



def test_conversion():
    import numpy as np

    def as_np(xyz):
        return np.array([xyz.x,xyz.y,xyz.z])

    a = XYZ(5,8,1)

    assert np.allclose(a.to_np,as_np(a))


def test_vectorXYZ_transformations():
    v = VectorXYZ([XYZ(0, 0, 0), XYZ(1, 1, 1)])
    v.transform(Translation3d(XYZ(2, 2, 2)))

    for (a, b) in zip(v, VectorXYZ([XYZ(2, 2, 2), XYZ(3, 3, 3)])):
        assert (a - b).len() == 0
    v.transform(Translation3d(XYZ(2, 2, 2)))
    v.transform(Transformation3d(Rotation3d(XYZ(1, 1, 1), Degrees(30)), Translation3d(XYZ(2, 2, 2))))
    v.transform(Rotation3d(XYZ(1, 1, 1), Degrees(30)))
    v.transform(UniformScale3d(5))


def test_vectorXYZ_numpy_conversions():
    from timeit import default_timer as timer


    vectors = np.random.random( (10000,3) )

    start = timer()
    v1 = VectorXYZ([XYZ(x,y,z) for x,y,z in vectors ])
    end = timer()
    t1 = end-start

    start = timer()
    v2 = VectorXYZ.from_numpy(vectors)
    end = timer()
    t2 = end-start

    print("Time(py) = %f" % t1)
    print("Time(c++)  = %f" % t2)
    print("Speedup   = %g%%" % (t1/t2*100))


    assert np.allclose(vectors, v2.to_numpy())








