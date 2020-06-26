import pytest
import os
from make_polygly import make_polyglycine

def test_dead_selection_access():
    from pyxmolpp2 import UniformScale, DeadFrameAccessError
    frame = make_polyglycine([("A", 20)])
    csel = frame.coords
    asel = frame.atoms
    rsel = frame.residues
    msel = frame.molecules

    del frame

    with pytest.raises(DeadFrameAccessError):
        asel[0]

    with pytest.raises(DeadFrameAccessError):
        asel[0:-1]

    with pytest.raises(DeadFrameAccessError):
        asel.filter(lambda a: True)

    with pytest.raises(DeadFrameAccessError):
        asel.coords

    with pytest.raises(DeadFrameAccessError):
        csel.apply(UniformScale(1))

    with pytest.raises(DeadFrameAccessError):
        asel.residues

    with pytest.raises(DeadFrameAccessError):
        asel.molecules

    del asel

    with pytest.raises(DeadFrameAccessError):
        rsel[0]

    with pytest.raises(DeadFrameAccessError):
        rsel[0:-1]

    with pytest.raises(DeadFrameAccessError):
        rsel.filter(lambda a: True)

    with pytest.raises(DeadFrameAccessError):
        rsel.atoms

    with pytest.raises(DeadFrameAccessError):
        rsel.molecules

    del rsel

    with pytest.raises(DeadFrameAccessError):
        msel[0]

    with pytest.raises(DeadFrameAccessError):
        msel[0:1]

    with pytest.raises(DeadFrameAccessError):
        msel.filter(lambda a: True)

    with pytest.raises(DeadFrameAccessError):
        msel.atoms

    with pytest.raises(DeadFrameAccessError):
        msel.residues
