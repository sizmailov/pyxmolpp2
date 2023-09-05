import sys
import warnings
from typing import List, Sequence, Union

from pyxmolpp2 import (
    XYZ,
    AtomPredicate,
    CoordSelection,
    Frame,
    Molecule,
    MoleculePredicate,
    Trajectory,
    Translation,
)
from pyxmolpp2._core._pipe import Align as _AlignCxx
from pyxmolpp2._core._pipe import WriteVectorsToCsv as _WriteVectorsToCsvCxx


class TrajectoryProcessor:
    def copy(self) -> "TrajectoryProcessor":
        raise NotImplementedError()

    def before_first_iteration(self, frame: Frame):
        raise NotImplementedError()

    def after_last_iteration(self, exc_type, exc_value, traceback) -> bool:
        return False

    def __call__(self, frame: Frame) -> Frame:
        raise NotImplementedError()

    def __or__(self, other: "TrajectoryProcessor"):
        return TrajectoryProcessorPair(self, other)

    def __ror__(self, lhs: Sequence[Frame]):
        if isinstance(lhs, ProcessedTrajectory):
            return ProcessedTrajectory(
                trajectory=lhs.trajectory,
                processor=TrajectoryProcessorPair(lhs.processor, self),
            )
        return ProcessedTrajectory(lhs, self)


class StatelessTrajectoryProcessor(TrajectoryProcessor):
    def before_first_iteration(self, frame: Frame):
        pass

    def copy(self):
        return self


class TrajectoryProcessorPair(StatelessTrajectoryProcessor):
    def __call__(self, frame: Frame) -> Frame:
        frame = self.first(frame)
        frame = self.second(frame)
        return frame

    def before_first_iteration(self, frame: Frame):
        self.first.before_first_iteration(frame)
        self.second.before_first_iteration(frame)

    def after_last_iteration(self, exc_type, exc_value, traceback) -> bool:
        exc_suppressed = self.second.after_last_iteration(
            exc_type, exc_value, traceback
        )
        if exc_suppressed:
            self.first.after_last_iteration(None, None, None)
        else:
            exc_suppressed = self.first.after_last_iteration(
                exc_type, exc_value, traceback
            )
        return exc_suppressed

    def __init__(self, first: TrajectoryProcessor, second: TrajectoryProcessor):
        self.first = first
        self.second = second


class ProcessedTrajectory:
    def __init__(
        self,
        trajectory: Union[Trajectory, Trajectory.Slice, "ProcessedTrajectory"],
        processor: TrajectoryProcessor,
    ):
        self.trajectory = trajectory
        self.processor = processor

    def __iter__(self):
        traj = iter(self.trajectory)
        frame = next(traj)
        self.processor.before_first_iteration(frame)
        try:
            yield self.processor(frame)
            for frame in traj:
                yield self.processor(frame)
        except:
            if not self.processor.after_last_iteration(*sys.exc_info()):
                raise
        else:
            self.processor.after_last_iteration(None, None, None)

    def __getitem__(self, index):
        if isinstance(index, slice):
            return ProcessedTrajectory(self.trajectory[index], self.processor.copy())
        else:
            return self.processor(self.trajectory[index])

    def __len__(self):
        return len(self.trajectory)

    @property
    def size(self):
        return len(self)

    @property
    def n_frames(self):
        return len(self)

    @property
    def n_atoms(self):
        return self.trajectory.n_atoms


class _AlignPython(TrajectoryProcessor):
    """Illustrates implementation of Align in pure python"""

    def __init__(
        self,
        by: AtomPredicate,
        reference: Frame = None,
        move_only: AtomPredicate = None,
    ):
        warnings.warn(
            "pipe._AlignPython serves illustration puposes only, use more performant pipe.Align instead"
        )
        self.align_atoms_selector = by
        self.reference = reference
        self.moved_atoms_selector = move_only

        self._reference = None
        self._frame_coords = None
        self._moved_coords = None
        self._ref_coords = None

    def before_first_iteration(self, frame: Frame):
        self._reference = self.reference or Frame(frame)
        self._frame_coords = frame.atoms.filter(self.align_atoms_selector).coords
        self._ref_coords = self._reference.atoms.filter(
            self.align_atoms_selector
        ).coords
        assert self._frame_coords.size == self._ref_coords.size
        if self.moved_atoms_selector:
            self._moved_coords = frame.atoms.filter(self.moved_atoms_selector).coords
        else:
            self._moved_coords = frame.coords

    def after_last_iteration(self, exc_type, exc_value, traceback) -> bool:
        self._reference = None
        self._frame_coords = None
        self._moved_coords = None
        self._ref_coords = None
        return False

    def __call__(self, frame: Frame):
        self._moved_coords.apply(self._frame_coords.alignment_to(self._ref_coords))
        return frame

    def copy(self):
        return Align(
            by=self.align_atoms_selector,
            reference=self.reference,
            move_only=self.moved_atoms_selector,
        )


class Align(_AlignCxx, TrajectoryProcessor):
    pass


class ScaleUnitCell(StatelessTrajectoryProcessor):
    def __init__(self, summary_volume_filename, column=1, max_rows=None):
        try:
            # Use faster pandas.read_csv() if available
            import pandas as pd

            self.volume = (
                pd.read_csv(
                    summary_volume_filename,
                    sep=r"\s+",
                    header=None,
                    usecols=[column],
                    nrows=max_rows,
                )
                .iloc[:, 0]
                .values
            )
        except ImportError:
            import numpy as np

            self.volume = np.genfromtxt(
                summary_volume_filename, usecols=[column], max_rows=max_rows
            )

    def __call__(self, frame: Frame):
        assert frame.index < len(
            self.volume
        ), "Frame index is greater than supplied volume array"
        frame.cell.scale_to_volume(self.volume[frame.index])
        return frame


class AssembleQuaternaryStructure(TrajectoryProcessor):
    def __init__(
        self, of: MoleculePredicate, by: AtomPredicate, reference: Frame = None
    ):
        self.molecules_selector = of
        self.reference = reference
        self.reference_atoms_selector = by

        self._reference = None
        self._ref_first_molecule_coords: CoordSelection = None
        self._molecules: List[Molecule] = []
        self._molecules_coords: List[CoordSelection] = []
        self._reference_mean_coords: List[XYZ] = []

    def before_first_iteration(self, frame: Frame):
        self._reference = self.reference or Frame(frame)
        mols = self._reference.molecules.filter(self.molecules_selector)
        self._reference_mean_coords = [
            mol.atoms.filter(self.reference_atoms_selector).coords.mean()
            for mol in mols
        ]
        self._ref_first_molecule_coords = (
            mols[0].atoms.filter(self.reference_atoms_selector).coords
        )
        assert (
            len(self._reference_mean_coords) > 1
        ), "Number of selected molecules must be greater than 1"

        self._molecules = [
            mol for mol in frame.molecules.filter(self.molecules_selector)
        ]
        self._molecules_coords = [
            mol.atoms.filter(self.reference_atoms_selector).coords
            for mol in self._molecules
        ]
        assert len(self._molecules_coords) == len(self._reference_mean_coords)
        assert frame.cell.volume > 1.5, "Did you forget to set periodic box?"

    def after_last_iteration(self, exc_type, exc_value, traceback) -> bool:
        self._reference = None
        self._ref_first_molecule_coords = None
        self._molecules = []
        self._molecules_coords = []
        self._reference_mean_coords = []
        return False

    def __call__(self, frame: Frame):
        # first molecule in assembly is aligned by convention
        alignment = self._ref_first_molecule_coords.alignment_to(
            self._molecules_coords[0]
        )
        # calculate reference coordinates for current frame
        ref_mean_coords = [alignment.transform(r) for r in self._reference_mean_coords]

        # shift rest of molecules to match reference coordinates as close as possible
        for mol_n in range(1, len(ref_mean_coords)):
            ref_point = ref_mean_coords[mol_n]
            mol = self._molecules[mol_n]
            mol_point = self._molecules_coords[mol_n].mean()
            closest = frame.cell.closest_image_to(ref_point, mol_point)
            if closest.shift.distance(XYZ()) > 0:
                mol.coords.apply(Translation(closest.shift))
        return frame

    def copy(self):
        return AssembleQuaternaryStructure(
            of=self.molecules_selector,
            by=self.reference_atoms_selector,
            reference=self.reference,
        )


class Run:
    """
    Serves for declarative trajectory processing style

    Example:
        >> traj | (processor_1 | processor_2) | Run()
    """

    def __ror__(self, trajectory):
        for _ in trajectory:
            pass


class WriteVectorsToCsv(_WriteVectorsToCsvCxx, TrajectoryProcessor):
    pass
