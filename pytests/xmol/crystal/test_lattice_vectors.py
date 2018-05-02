import pytest
import math

def test_calc_rmsd():
    from pyxmolpp2.geometry import XYZ, AngleValue
    from pyxmolpp2.crystal import LatticeVectors, BestShiftFinder


    latticeVectors = LatticeVectors(XYZ(1,4,1),XYZ(5,1,1),XYZ(7,1,4))

    bestShiftFinder = BestShiftFinder(latticeVectors)

    latticeVectors.scale_by(0.5)
    bestShiftFinder.scale_lattice_by(0.5)

    ref = XYZ(0,0,0)
    var = latticeVectors.translate(ref,1,4,43)
    print()
    print(var.x,var.y,var.z)
    dr, shift = bestShiftFinder.find_best_shift(ref,var)
    print(shift.x,shift.y,shift.z)
    var = var + shift

    assert var.x == pytest.approx(ref.x)
    assert var.y == pytest.approx(ref.y)
    assert var.z == pytest.approx(ref.z)
    assert dr == pytest.approx(0)




