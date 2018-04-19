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


def test_calc_alignment():
    from pyxmolpp2.geometry import calc_alignment, XYZ, VectorXYZ, calc_rmsd, Rotation3d, Translation3d, Degrees

    a = VectorXYZ([XYZ(1,2,3),XYZ(1,2,5),XYZ(4,2,7),XYZ(8,1,4)])
    G = Rotation3d(XYZ(7,6,5),Degrees(12))*Translation3d(XYZ(8,-9,1))
    b = VectorXYZ([ G.transform(x) for x in a ])



    G2 = calc_alignment(a,b)
    assert calc_rmsd(a,b) > 1
    assert calc_rmsd(a,b,G2) == pytest.approx(0)





