import pytest
import math

def test_closed_image():
    from pyxmolpp2.v1 import UnitCell, XYZ


    latticeVectors = UnitCell(XYZ(1,4,1),XYZ(5,1,1),XYZ(7,1,4))

    cell.scale_by(0.5)
    cell.scale_lattice_by(0.5)

    ref = XYZ(0,0,0)
    var = latticeVectors.translate(ref,1,4,43)
    # print()
    # print(var.x,var.y,var.z)
    result = cell.closet_image_to(ref,var)
    # print(shift.x,shift.y,shift.z)
    var = var + shift

    assert var.x == pytest.approx(ref.x)
    assert var.y == pytest.approx(ref.y)
    assert var.z == pytest.approx(ref.z)
    assert dr == pytest.approx(0)




