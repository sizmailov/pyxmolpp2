import pytest
import math

from pyxmolpp2.geometry import *

def test_conversions():

    deg = Degrees(10)
    rad = Radians(100)

    assert rad.radians == pytest.approx(100)
    assert deg.degrees == pytest.approx(10)

    assert rad.degrees == pytest.approx(radians_to_degrees(100))
    assert deg.radians == pytest.approx(degrees_to_radians(10))

def test_operations():

    l = Radians(10)
    r = Radians(20)


    assert (l + r).radians == pytest.approx(l.radians+r.radians)
    assert (l - r).radians == pytest.approx(l.radians-r.radians)
    assert (l * 3).radians == pytest.approx(l.radians*3)
    assert (3 * l).radians == pytest.approx(3*l.radians)
    assert (l/3).radians == pytest.approx(l.radians/3)
    assert l.to_standard_range().radians == pytest.approx(10-2*2*math.pi)
    assert (-l).to_standard_range().radians == pytest.approx(-10+2*2*math.pi)
    assert (-l).radians == pytest.approx(-(l.radians))
    assert l < r
    assert l <= r
    assert r > l
    assert r >= l

    assert sin(l) == pytest.approx(math.sin(l.radians))
    assert cos(l) == pytest.approx(math.cos(l.radians))
    assert tan(l) == pytest.approx(math.tan(l.radians))
    assert l.radians == pytest.approx(fabs(l).radians)
    assert l.radians == pytest.approx(fabs(-l).radians)












