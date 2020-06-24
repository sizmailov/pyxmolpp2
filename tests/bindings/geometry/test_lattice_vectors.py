import pytest


def test_unit_cell_volume():
    from pyxmolpp2 import UnitCell, XYZ, Degrees

    cell = UnitCell(1, 1, 1, Degrees(90), Degrees(90), Degrees(90))
    assert cell.volume == 1
    cell.scale_by(2)
    assert cell.volume == 8

    cell.scale_to_volume(27)

    assert cell[0].distance(XYZ(3, 0, 0)) == pytest.approx(0)
    assert cell[1].distance(XYZ(0, 3, 0)) == pytest.approx(0)
    assert cell[2].distance(XYZ(0, 0, 3)) == pytest.approx(0)

def test_from_rst7_conversion():
    from pyxmolpp2 import UnitCell
    line = "  59.3881247  59.3881247  59.3881247 109.4712190 109.4712190 109.4712190"
    cell: UnitCell = UnitCell.from_rst7_line(line)
    print(cell[0], cell[1], cell[2])
    print(cell[0].len(), cell[1].len(), cell[2].len())
    print(">>", cell.a, cell.b,cell.c)
    print(">>", cell.alpha.degrees, cell.beta.degrees, cell.gamma.degrees)
    assert cell.a == pytest.approx(59.3881247)
    assert cell.b == pytest.approx(59.3881247)
    assert cell.c == pytest.approx(59.3881247)
    assert cell.alpha.degrees == pytest.approx(109.4712190)
    assert cell.beta.degrees == pytest.approx(109.4712190)
    assert cell.gamma.degrees == pytest.approx(109.4712190)
    with pytest.raises(RuntimeError):
        cell = UnitCell.from_rst7_line("too_short")

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
