
from pyxmolpp2.trajectory import Trajectory
from pyxmolpp2.trjtool import DatFile
from pyxmolpp2.amber import NetCDFTrajectoryFile
from pyxmolpp2.pdb import PdbFile, \
    AlteredPdbRecords, \
    StandardPdbRecords, \
    RecordName, \
    FieldName
from tqdm import tqdm
altered_records = AlteredPdbRecords(StandardPdbRecords.instance())
altered_records.alter_record(RecordName("ATOM"), FieldName("serial"), [7, 12])

st = PdbFile("/home/sergei/Downloads/chromatin/run00001.pdb", altered_records).get_frame()
traj = Trajectory(st, check_portions_to_match_reference=False)


for i in range(10):
    filename = f"/home/sergei/Downloads/chromatin/run{1:05d}.dat"
    traj.push_trajectory_portion(DatFile(filename))

ats = None
geom_centers = []
for frame in tqdm(traj):
    if ats is None:
        ats = frame.asAtoms
    geom_centers.append(ats.toCoords.to_numpy().mean(axis=-1))

