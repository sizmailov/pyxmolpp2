import pytest


def test_closed_image():
    from pyxmolpp2 import UnitCell, XYZ

    cell = UnitCell(XYZ(1, 4, 1), XYZ(5, 1, 1), XYZ(7, 1, 4))

    cell.scale_by(0.5)

    ref = XYZ(0, 0, 0)
    var = ref + cell.translation_vector(1, 4, 43)
    # print()
    # print(var.x,var.y,var.z)
    result = cell.closest_image_to(ref, var)
    # print(shift.x,shift.y,shift.z)


    assert result.pos.x == pytest.approx(ref.x)
    assert result.pos.y == pytest.approx(ref.y)
    assert result.pos.z == pytest.approx(ref.z)
    assert result.distance == pytest.approx(0)
