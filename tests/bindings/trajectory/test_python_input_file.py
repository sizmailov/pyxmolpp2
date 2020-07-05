from make_polygly import make_polyglycine
from pyxmolpp2 import TrajectoryInputFile, AtomSpan, Trajectory, UnitCell
import numpy as np


class IotaTrajectory(TrajectoryInputFile):
    def __init__(self, natoms, nframes):
        super().__init__()
        self._natoms = natoms
        self._nframes = nframes

    def n_frames(self):
        return self._nframes

    def n_atoms(self):
        return self._natoms

    def read_frame(self, index: int, coords: AtomSpan, cell: UnitCell):
        coords.values[:] = np.ones_like(coords.values) * index
        cell.scale_to_volume(index + 1)

    def advance(self, shift: int):
        pass

def test_pseudo_trajectory():
    ref = make_polyglycine([('A', 10)])

    traj = Trajectory(ref)
    traj.extend(IotaTrajectory(natoms=ref.atoms.size, nframes=10))

    for frame in traj:
        assert np.allclose(frame.coords.values, frame.index)
        # assert np.isclose(frame.cell.volume, frame.index + 1) # fixme: enable
