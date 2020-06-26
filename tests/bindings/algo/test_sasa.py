import pytest
import os

def test_calc_sasa():
    from pyxmolpp2 import PdbFile, calc_sasa
    from timeit import default_timer as timer
    import numpy as np
    import glob

    for filename in sorted(glob.glob(os.environ["TEST_DATA_PATH"] + "/pdb/rcsb/*.pdb")):
        frame = PdbFile(filename).frames()[0]
        coords = frame.coords.values
        radii = np.array([1.0] * frame.atoms.size)

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

