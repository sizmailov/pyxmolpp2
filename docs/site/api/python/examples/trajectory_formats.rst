Reading trajectories
####################


:ref-prefix:
    pyxmolpp2


Trajectory construction is two-step process.

First you need to provide a reference topology as :ref:`Frame` (usually read from ``.pdb`` file).
The coordinates of reference frame are not used in trajectory traverse.

Next you need to add files to trajectory by calling :ref:`Trajectory.extend`.
The supplied instances must be derived from :ref:`TrajectoryInputFile`.


To keep example self-contained, lets import all needed classes and define paths to trajectories to read.

.. py-exec::
    :context-id: read_trajectory

    import os
    from pyxmolpp2 import PdbFile, Trajectory, TrjtoolDatFile, GromacsXtcFile, AmberNetCDF

    dat_path = os.path.join(os.environ["TEST_DATA_PATH"], "trjtool/GB1/")
    nc_path = os.path.join(os.environ["TEST_DATA_PATH"],
                           "amber/1ubq_intolerant_shake_ewald_SPCE/")
    xtc_path = os.path.join(os.environ["TEST_DATA_PATH"], "gromacs/xtc/")


Composing a trajectory isn't very different for all formats:

.. py-exec::
    :context-id: read_trajectory

    # Trjtool .dat format
    dat_traj = Trajectory(PdbFile(dat_path + "/run00001.pdb").frames()[0])
    dat_traj.extend(TrjtoolDatFile(dat_path + "/run00001.dat"))

    # Amber NetCDF format
    nc_traj = Trajectory(PdbFile(nc_path + "/box.pdb").frames()[0])
    nc_traj.extend(AmberNetCDF(nc_path + "/run00001.0-10.nc"))

    # Gromacs xtc format
    xtc_traj = Trajectory(PdbFile(xtc_path + "/1am7_protein.pdb").frames()[0])
    xtc_traj.extend(GromacsXtcFile(xtc_path + "/1am7_corrected.xtc", n_frames=51))
    # note: GromacsXtcFile constructor requires number of
    #       frames to be known ahead of time to avoid file re-read


Now all trajectories are ready to work:

.. py-exec::
    :context-id: read_trajectory
    :discard-context:

    for fmt, traj in [
        ("Amber NetCDF", nc_traj),
        ("Gromacs XTC", xtc_traj),
        ("Trjtool", dat_traj)
    ]:
        print(f"{fmt}: {traj.n_atoms} atoms, {traj.n_frames} frames")
        for frame in traj[:3]:
            print(f"    index={frame.index:2},"
                  f" volume={frame.cell.volume:15.10g},"
                  f" geom_center={frame.coords.mean()}")
        print()
