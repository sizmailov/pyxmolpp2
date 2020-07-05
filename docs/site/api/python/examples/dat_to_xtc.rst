Convert to xtc format
#####################

In this example we read 2-nanosecond trajectory in ``.dat`` format and write as single ``.xtc`` file with
various precision to compare different levels of lossy compression.

.. py-exec::

    import os
    from pyxmolpp2 import PdbFile, Trajectory, TrjtoolDatFile, XtcWriter

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "trjtool/GB1/run00001.pdb")
    path_to_traj = os.path.join(os.environ["TEST_DATA_PATH"], "trjtool/GB1/")
    frame = PdbFile(pdb_filename).frames()[0]


    # Read trajectory and calculate net size in bytes
    traj = Trajectory(frame)
    dat_size = 0
    for i in range(1, 3):
        dat_filename = f"{path_to_traj}/run{i:05d}.dat"
        traj.extend(TrjtoolDatFile(dat_filename))
        dat_size += os.path.getsize(dat_filename)


    # Convert trajectory to .xtc and collect resulting sizes
    sizes = {}
    for precision in [100, 1000, 10000]:
        xtc_filename = f"trajectory_{precision}.xtc"
        xtc_writer = XtcWriter(xtc_filename, precision=precision)
        for frame in traj:
            xtc_writer.write(frame)
        del xtc_writer # closes file
        del frame # to re-enter trajectory
        sizes[precision] = os.path.getsize(xtc_filename)
        os.remove(xtc_filename) # clean-up

    # Print results
    print(f"                    .dat size: {dat_size:9d} bytes (100%)")
    for precision, size in sizes.items():
        print(f".xtc size ({10/precision:5.3f} Å precision): "
              f"{size:9d} bytes ({size/dat_size*100:3.0f}%)")



