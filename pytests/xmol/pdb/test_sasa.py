import pytest


def test_calc_sasa():
    from pyxmolpp2.pdb import PdbFile
    from pyxmolpp2.geometry import calc_sasa
    from timeit import default_timer as timer
    import numpy as np
    import glob

    for filename in sorted(glob.glob("tests_dataset/pdb/rcsb/*.pdb")):
        frame = PdbFile(filename).get_frame()
        ats = frame.asAtoms
        radii = np.array([1.0] * ats.size)
        coords = ats.toCoords

        t1 = timer()
        assert calc_sasa(coords, radii, 0.0, np.array([0], dtype=np.intc)).size == 1
        t2 = timer()
        assert calc_sasa(coords, radii, 0.0, np.array([0, 1, 2, 3, 4], dtype=np.intc)).size == 5
        t3 = timer()
        assert calc_sasa(coords, radii, 0.0).size == radii.size
        t4 = timer()
        assert calc_sasa(coords, radii, 0.0, n_samples=1).size == radii.size

        T1, T3 = t2 - t1, t4 - t3
        assert T3 > T1

