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