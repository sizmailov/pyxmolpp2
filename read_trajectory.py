import pyxmolpp2
from pyxmolpp2.v1 import *
from tqdm import tqdm
import numpy as np

print(pyxmolpp2)

st = PdbFile("/home/sergei/Downloads/chromatin/run00001.pdb").frames()[0]
traj = Trajectory(st)


class MyTrajectoryFile(TrajectoryInputFile):
    def __init__(self, ):
        super().__init__()

    def n_atoms(self):
        return 25474

    def n_frames(self):
        return 4

    def advance(self, n):
        print("advance")

    def read_coordinates(self, index, coords):
        print("read_coordinates", index, coords)


for i in range(10):
    filename = f"/home/sergei/Downloads/chromatin/run{1:05d}.dat"
    traj.extend(TrjtoolDatFile(filename))

centers = []

for frame in tqdm(traj):
    pass
    centers += [frame.coords.values.T[::1].mean(axis=0)]

print(len(centers), centers)