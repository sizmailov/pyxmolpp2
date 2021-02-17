from typing import Sequence
from pyxmolpp2 import Frame, PdbFile, TrjtoolDatFile, Trajectory, XYZ
from pyxmolpp2.pipe import ProcessedTrajectory, Align, ScaleUnitCell, TrajectoryProcessor
import os
import numpy as np
from trajectory.test_python_input_file import IotaTrajectory
from make_polygly import make_polyglycine


class AngstromsToNanometers(TrajectoryProcessor):
    def __init__(self):
        self.frame_coords = None

    def before_first_iteration(self, frame: Frame):
        self.frame_coords = frame.coords

    def after_last_iteration(self, exc_type, exc_value, traceback) -> bool:
        self.frame_coords = None
        return False

    def __call__(self, frame: Frame):
        self.frame_coords.values[:] = self.frame_coords.values * 10
        return frame

    def copy(self):
        return AngstromsToNanometers()


def test_pipes():
    ref = PdbFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.pdb").frames()[0]

    traj = Trajectory(ref)
    traj.extend(TrjtoolDatFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.dat"))

    first_geom_center = XYZ(8.422286, 0.967190, -13.856332)

    for frame in traj:
        assert frame.coords.mean().distance(first_geom_center) < 0.001
        break
    del frame

    for frame in traj | AngstromsToNanometers():
        assert frame.coords.mean().distance(first_geom_center * 10) < 0.001
        break
    del frame

    for frame in traj | Align(by=lambda a: True):
        assert frame.coords.mean().distance(first_geom_center) < 0.001
    del frame
    for frame in traj[0:10] | Align(by=lambda a: True):
        assert frame.coords.mean().distance(first_geom_center) < 0.001
    del frame
    for frame in traj[0:10] | Align(by=lambda a: True) | AngstromsToNanometers():
        assert frame.coords.mean().distance(first_geom_center * 10) < 0.001
    del frame


def test_pipe_slice():
    ref = make_polyglycine([('A', 10)])

    traj = Trajectory(ref)
    traj.extend(IotaTrajectory(natoms=ref.atoms.size, nframes=10))

    for frame in (traj | AngstromsToNanometers())[:10]:
        assert np.allclose(frame.coords.values / 10, frame.index)
        assert np.isclose(frame.time, frame.index * 15)


def test_unit_cell_scale():
    ref = PdbFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.pdb").frames()[0]
    traj = Trajectory(ref)
    traj.extend(TrjtoolDatFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00002.dat"))
    volumes = []
    for frame in traj[::100] | ScaleUnitCell(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/summary/summary.VOLUME"):
        volumes += [frame.cell.volume]

    assert np.allclose(volumes,
                       [112190.6817, 111667.0223, 112060.3447, 112010.4530, 111899.7620, 112420.6936, 111794.6408,
                        112093.9081, 111655.2990, 111978.6604])


def test_tqdm():
    from tqdm import tqdm
    ref = PdbFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.pdb").frames()[0]

    traj = Trajectory(ref)
    traj.extend(TrjtoolDatFile(os.environ["TEST_DATA_PATH"] + "/trjtool/GB1/run00001.dat"))
    for _ in tqdm(traj):
        pass
    del _
    for _ in tqdm(traj[::2]):
        pass
    del _