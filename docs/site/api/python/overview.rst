:ref-prefix:
    pyxmolpp2

.. role:: cpp(code)
    :language: c++
.. role:: py(code)
    :language: py
.. role:: sh(code)
    :language: sh


.. |Atom| replace:: :ref:`Atom`
.. |Residue| replace:: :ref:`Residue`
.. |Molecule| replace:: :ref:`Molecule`
.. |Frame| replace:: :ref:`Frame`

.. |CoordSelection| replace:: :ref:`CoordSelection`
.. |AtomSelection| replace:: :ref:`AtomSelection`
.. |ResidueSelection| replace:: :ref:`ResidueSelection`
.. |MoleculeSelection| replace:: :ref:`MoleculeSelection`

.. py-exec::
    :context-id: FMRA
    :hide-code:

    import os
    from pyxmolpp2 import PdbFile, Frame, aName
    filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
    frame = PdbFile(filename).frames()[0]

Overview
########

Basics
======

The ``pyxmolpp2`` library implements |Frame|/|Molecule|/|Residue|/|Atom| hierarchy to represent a molecular frame.

Unlike many other molecular libraries |Atom|, |Residue| or |Molecule| instances do not exist in isolation, they are always part of |Frame|.
Therefore any |Atom|, |Residue| or |Molecule| are guaranteed to have a parent.
This makes expression :py:`atom.residue.molecule.frame.index` always valid and eliminates :py:`is not None` checks from user and library code.

.. block-danger::
    Keep frame alive

    |Atom|/|Residue|/|Molecule| are alive while the |Frame| exists.
    |Frame| exists until python holds a reference to it. No frame - no game.


.. block-default:: Example

    Let's access first C-alpha atom of molecule ``A``:

    .. py-exec::
        :context-id: FMRA

        atom = frame["A"][1]["CA"]
        print(f"{atom.id} {atom.name} {atom.r}",
              f"{atom.residue.id} {atom.residue.name}",
              atom.molecule.name,
              sep='\n'
        )


    Let's intentionally destroy frame, by dropping the only reference and see what
    happens when we try to access stored atom reference:

    .. py-exec::
        :context-id: FMRA
        :raises: DeadFrameAccessError

        frame = None
        atom.name

    .. py-exec::
        :context-id: FMRA
        :hide-code:

        frame = PdbFile(filename).frames()[0]


To ease manipulation with group of elements ``pyxmolpp2`` provides number of *selection* classes: |CoordSelection|, |AtomSelection|, |ResidueSelection| and |MoleculeSelection|. Selections support mutual interconversion operations, generic set operations (union, intersection, difference), slicing, iteration and number of other handy methods.

Selections are *ordered* sets of elements, the order in selections matches order in parent frame.
Note, on mixing elements from two frames an exception will be raised, see an `example <mixing-frames-in-selection_>`_.


.. block-default:: Examples

    Count atoms, residues and molecules in frame

    .. py-exec::
        :context-id: FMRA

        print(f"""\
            Molecules: {frame.molecules.size}
            Residues : {frame.residues.size}
            Atoms    : {frame.atoms.size}
            Coords   : {frame.coords.size} (always same as atoms.size)
        """)

    Combine and convert selections:

    .. py-exec::
        :context-id: FMRA

        first_two = frame["A"].residues[:2]
        last_two = frame["A"].residues[-2:]
        four_termini = first_two | last_two
        all_ca = frame.atoms.filter(aName=="CA")
        print(f"""\
            Atoms in two first residues : {first_two.atoms.size}
            Atoms in two last residues  : {last_two.atoms.size}
            Atoms in four residues      : {four_termini.atoms.size}
            Number of "CA" atoms        : {all_ca.size}
            Geom center of "CA" atoms   : {all_ca.coords.mean()}
        """)

    .. _mixing-frames-in-selection:

    Mixing elements from two or more frames is not allowed:

    .. py-exec::
        :hide-code:
        :context-id: FMRA

        frame2 = Frame(frame)

    .. py-exec::
        :context-id: FMRA
        :raises: MultipleFramesSelectionError

        mixed_atom_selection = frame.atoms | frame2.atoms

Predicates
----------

Library provides predicate generators in order to simplify selection filtering. On comparison with value they produce
predicates which can be combined together to produce new one. Parent predicates can be naturally applied to selections of child elements, for example :ref:`ResiduePredicate` can be applied to filter :ref:`AtomSelection`.

.. py-exec::
    :context-id: FMRA

    from pyxmolpp2 import aName, rId, rName, aId, mName

    for predicate in [
        aName=="CA",
        aId == 3,
        # combine AtomPredicate and ResiduePredicate:
        (aName.is_in({"N", "CA", "C", "O"})) & rId.is_in({1, 2, 3}),
        rName=="GLY",
    ]:
        asel = frame.atoms.filter(predicate)
        print(f"Selected {asel.size:2d} atoms from "
              f"{asel.residues.size:2d} residues"
              f" by {type(predicate).__name__}")



Span vs Selection
-----------------

There are two slightly different types of "selections" in the library.
A ``Span`` is a continuous selections of elements, while ``Selection`` is an arbitrary set of elements.
Those two forms of selections functionally almost the same so you won't notice any difference for most of the time.
For more details check API reference.

On this page I make no distinction between two.

.. py-exec::
    :context-id: FMRA
    :discard-context:

    print(frame.atoms[10:20])   # AtomSpan
    print(frame.atoms[10:20:2]) # AtomSelection


Trajectory
==========

A trajectory represents an evolution of |Frame| in time. It needs a reference topology provided by initial |Frame| and
number of input coordinate files.


.. py-exec::
    :context-id: trajectory
    :hide-code:

    import os
    from pyxmolpp2 import PdbFile, Trajectory, TrjtoolDatFile, Translation, aName

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "trjtool/GB1/run00001.pdb")
    path_to_traj = os.path.join(os.environ["TEST_DATA_PATH"], "trjtool/GB1/")
    frame = PdbFile(pdb_filename).frames()[0]

Let's construct out trajectory from trjtool ``.dat`` files (:ref:`TrjtoolDatFile`)

.. py-exec::
    :context-id: trajectory

    traj = Trajectory(frame)
    for i in range(1, 3):
        traj.extend(TrjtoolDatFile(f"{path_to_traj}/run{i:05d}.dat"))
    print(traj.size)

Trajectory supports index access and slices:

.. py-exec::
    :context-id: trajectory

    frame_10 = traj[10] # returns copy of frame
    print(len(traj[:100]))


On iteration over trajectory (or its slice) a copy of frame is created at the beginning and updated on every step.

.. py-exec::
    :context-id: trajectory

    for f in traj[::250]:
        print(f"{f.index:4d}", f.coords.mean())

Trajectory does not support simultaneous iterations and keeps track of iterators created.
To re-enter trajectory it's required to release all references to iteration variable from previous run.

.. py-exec::
    :context-id: trajectory
    :hl_lines: 1
    :class: m-inverted

    del f  # release trajectory iterator reference
    for f in traj[::500]:
        print(f"{f.index:4d}", f.coords.mean())

If you forget to do so an exception will be raised.

.. py-exec::
    :context-id: trajectory
    :raises: TrajectoryDoubleTraverseError

    for f in traj[::500]:
        print(f.index, f.coords.mean())

.. note-warning::

    Note that index-based access to trajectory counts as a 1-size iteration.


Pipe-processing
---------------

Common pre-processing operations are available in :ref:`pyxmolpp2.pipe`.
For example, we often need all frames in trajectory to be aligned by subset of atoms. We can use conveniently pre-processing
trajectory by :ref:`pipe.Align`:


.. py-exec::
    :hide-code:
    :context-id: trajectory

    traj = Trajectory(frame)
    for i in range(1, 3):
        traj.extend(TrjtoolDatFile(f"{path_to_traj}/run{i:05d}.dat"))


.. py-exec::
    :context-id: trajectory
    :discard-context:
    :hl_lines: 3
    :class: m-inverted

    from pyxmolpp2.pipe import Align

    for f in traj[::500] | Align(by=aName=="CA"):
        print(f"{f.index:4d}", f.coords.mean())

Such "pipe" processors can be chained together which makes this scheme very flexible.