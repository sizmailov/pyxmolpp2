def test_read_chromatin():
    import pyxmolpp2
    from tqdm import tqdm
    print(pyxmolpp2)
    from pyxmolpp2 import PdbFile, TrjtoolDatFile, Trajectory

    st = PdbFile("/home/sergei/Downloads/chromatin/run00001.pdb").frames()[0]
    traj = Trajectory(st)

    for i in range(1, 2):
        filename = f"/home/sergei/Downloads/chromatin/run{i:05d}.dat"
        # print(filename)
        traj.extend(TrjtoolDatFile(filename))
    for frame in tqdm(traj):
        # print(frame.coords.values)
        print(frame.coords.values.flags)
        break
        # if frame.index>1:
        #     break
        pass
