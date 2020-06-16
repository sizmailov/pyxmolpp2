import pytest
from make_polygly import make_polyglycine


@pytest.mark.skip("not implemented")
def test_selection_bool_indexing():
    import numpy as np
    frame = make_polyglycine([("A", 20)])
    asel = frame.atoms

    ind = np.array([a.name.str == "C" for a in asel])

    subset = asel[ind]

    assert subset.size == 20


@pytest.mark.skip("not implemented")
def test_selection_int_indexing():
    import numpy as np
    from pyxmolpp2 import aName
    frame = make_polyglycine([("A", 20)])
    asel = frame.atoms
    rsel = frame.residues
    csel = frame.molecules

    ind = np.array([i for i, a in enumerate(asel) if a.name == "C"])
    ind2 = asel.filter(aName == "C").index
    assert np.allclose(ind, ind2)

    subset = asel[ind]
    assert subset.size == 20
    assert asel[ind2].size == 20

    # Bool array size doesn't match selection size
    with pytest.raises(RuntimeError):
        asel[np.array([True, False])]

    with pytest.raises(RuntimeError):
        rsel[np.array([True, False])]

    with pytest.raises(RuntimeError):
        csel[np.array([True, False])]

    # Bool array element type is not integer or bool
    with pytest.raises(RuntimeError):
        asel[np.array([1.0, 2.0])]

    with pytest.raises(RuntimeError):
        rsel[np.array([1.0, 2.0])]

    with pytest.raises(RuntimeError):
        csel[np.array([1.0, 2.0])]

    # Array dimension is not 1
    with pytest.raises(RuntimeError):
        asel[np.array([[0, 2], [1, 2]])]

    with pytest.raises(RuntimeError):
        rsel[np.array([[0, 2], [1, 2]])]

    with pytest.raises(RuntimeError):
        csel[np.array([[0, 2], [1, 2]])]

    assert frame.residues[np.array([0, 1])].size == 2
    assert frame.residues[np.array([0])].size == 1  # array index -> selection
    assert frame.chains[np.array([], dtype=int)].size == 0  # array index -> selection
    assert frame.chains[0].size == 20  # int index -> Chain


if __name__ == "__main__":
    test_selection_bool_indexing()
    test_selection_int_indexing()
