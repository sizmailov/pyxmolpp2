import pytest
import os
from make_polygly import make_polyglycine

def test_dead_selection_access():
    from pyxmolpp2 import UniformScale
    frame = make_polyglycine([("A", 20)])
    asel = frame.atoms
    rsel = frame.residues
    csel = frame.molecules

    del frame

    with pytest.raises(RuntimeError):
        asel[0]

    with pytest.raises(RuntimeError):
        asel[0:-1]

    with pytest.raises(RuntimeError):
        asel.filter(lambda a: True)

    with pytest.raises(RuntimeError):
        asel.coords

    # with pytest.raises(RuntimeError):
    #     asel.apply(UniformScale(1))

    with pytest.raises(RuntimeError):
        asel.residues

    with pytest.raises(RuntimeError):
        asel.molecules

    del asel

    with pytest.raises(RuntimeError):
        rsel[0]

    with pytest.raises(RuntimeError):
        rsel[0:-1]

    with pytest.raises(RuntimeError):
        rsel.filter(lambda a: True)

    with pytest.raises(RuntimeError):
        rsel.atoms

    with pytest.raises(RuntimeError):
        rsel.molecules

    del rsel

    with pytest.raises(RuntimeError):
        csel[0]

    with pytest.raises(RuntimeError):
        csel[0:1]

    with pytest.raises(RuntimeError):
        csel.filter(lambda a: True)

    with pytest.raises(RuntimeError):
        csel.atoms

    with pytest.raises(RuntimeError):
        csel.residues
