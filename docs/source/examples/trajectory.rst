Trajectory from path
====================


.. code-block:: python



    def make_trajectory(path_to_dir, nres_to_keep, limit=None, stride=None):

        from pyxmolpp2.trajectory import Trajectory
        from pyxmolpp2.trjtool import DatFile
        from pyxmolpp2.pdb import PdbFile,\
                                  AlteredPdbRecords,\
                                  StandardPdbRecords,\
                                  RecordName,\
                                  FieldName

        import glob

        altered_records = AlteredPdbRecords(StandardPdbRecords.instance())
        altered_records.alter_record(RecordName("ATOM"), FieldName("serial"), [7, 12])


        pdb_file = PdbFile(path_to_dir+"/1_build/box.pdb")
        ref = pdb_file.get_frame(altered_records)
        box_to_ref(ref, nres_to_keep=nres_to_keep)
        dat_files = sorted(glob.glob(os.path.join(path_to_dir,"5_run","*.dat")))

        traj = Trajectory(ref, True)

        for dat in dat_files[:limit:stride]:
            traj.push_trajectory_portion(DatFile(dat))

        return traj, ref
