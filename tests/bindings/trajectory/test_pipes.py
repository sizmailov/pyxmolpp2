from typing import Sequence, Callable
from pyxmolpp2 import Frame, AtomPredicate, PdbFile, TrjtoolDatFile, Trajectory, XYZ


class ProcessedTrajectory:
    def __init__(self, trajectory: Sequence[Frame], processor: Callable[[Frame], None]):
        self.trajectory = trajectory
        self.processor = processor

    def __iter__(self):
        for frame in self.trajectory:
            self.processor(frame)
            yield frame

    def __getitem__(self, index):
        return self.trajectory[index]


class Align:
    def __init__(self, by: AtomPredicate, reference: Frame = None, move_only: AtomPredicate = None):
        self.by = by
        self.reference = reference
        self.move_only = move_only
        self.frame_coords = None
        self.moved_coords = None
        self.ref_coords = None

    def __ror__(self, trajectory: Sequence[Frame]):
        if self.reference is None:
            self.reference = trajectory[0]
        return ProcessedTrajectory(trajectory, self)

    def __call__(self, frame: Frame):
        if self.frame_coords is None:
            self.frame_coords = frame.atoms.filter(self.by).coords
            self.ref_coords = self.reference.atoms.filter(self.by).coords
            assert self.frame_coords.size == self.ref_coords.size
            if self.move_only:
                self.moved_coords = frame.atoms.filter(self.move_only).coords
            else:
                self.moved_coords = frame.coords
        self.moved_coords.apply(self.frame_coords.alignment_to(self.ref_coords))


class AngstromsToNanometers:
    def __init__(self):
        self.frame_coords = None

    def __ror__(self, trajectory: Sequence[Frame]):
        return ProcessedTrajectory(trajectory, self)

    def __call__(self, frame: Frame):
        if self.frame_coords is None:
            self.frame_coords = frame.coords
        self.frame_coords.values[:] = self.frame_coords.values * 10


def test_pipes():
    ref = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb").frames()[0]

    traj = Trajectory(ref)
    traj.extend(TrjtoolDatFile("tests_dataset/trjtool/GB1/run00001.dat"))

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


def test_tqdm():
    from tqdm import tqdm
    ref = PdbFile("tests_dataset/trjtool/GB1/run00001.pdb").frames()[0]

    traj = Trajectory(ref)
    traj.extend(TrjtoolDatFile("tests_dataset/trjtool/GB1/run00001.dat"))
    for _ in tqdm(traj):
        pass
    del _
    for _ in tqdm(traj[::2]):
        pass
    del _