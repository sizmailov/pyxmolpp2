
RMSD/RMSF
^^^^^^^^^


.. py-exec::
    :context-id: RMSD

    import os
    from pyxmolpp2 import calc_rmsd, calc_alignment, PdbFile

Let's create a frame to work with

.. py-exec::
    :context-id: RMSD

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/5BMG.pdb")
    pdb_file = PdbFile(pdb_filename)
    frame = pdb_file.frames()[0]

Number of residues in molecules must be same (strip water, ions, etc.)

.. py-exec::
    :context-id: RMSD

    N = 0
    for i in range(frame.size):
        if frame.molecules[i].size != frame.asChains[0].size:
            break
        N += 1

Print RMSD matrix for all deposited chains:

.. py-exec::
    :context-id: RMSD

    for i in range(0, N):
        chain_i_ca = frame.molecules[i].atoms.filter(aName == "CA")

        for j in range(0, i + 1):
            chain_j_ca = frame.molecules[j].atoms.filter(aName == "CA")

            chain_i_ca.coords.alignment_to(chain_j_ca.coords)
            rmsd = calc_rmsd(chain_i_ca.coords, chain_j_ca.coords, alignment)

            print("%5.1f" % rmsd, end=" ")

        print()


Calculate RMSF per residue

.. py-exec::
    :context-id: RMSD

    from pyxmolpp2.geometry import UniformScale3d

    first_chain_ca = frame.asChains[0].asAtoms.filter(aName == "CA")

    # initialize average coordinates with (0,0,0)
    avg_coords = first_chain_ca.toCoords.transform(UniformScale3d(0))

    # calculate average coordinates across N frames
    for i in range(0, N):
        chain_i_ca = frame.asChains[i].asAtoms.filter(aName == "CA")
        chain_i_ca.transform(calc_alignment(first_chain_ca.toCoords, chain_i_ca.toCoords))

        for k, a in enumerate(chain_i_ca):
            avg_coords[k] += a.r

    avg_coords.transform(UniformScale3d(1/N))

    # align to average coordinates
    for i in range(0, N):
        chain_i_ca = frame.asChains[i].asAtoms.filter(aName == "CA")
        chain_i_ca.transform(calc_alignment(avg_coords, chain_i_ca.toCoords))

    # calculate per residue RMSF

    import numpy as np

    rmsf = np.zeros((first_chain_ca.size,) )
    for i in range(0, N):
        chain_i_ca = frame.asChains[i].asAtoms.filter(aName == "CA")
        for k, a in enumerate(chain_i_ca):
              rmsf[k] += (a.r-avg_coords[k]).len2()

    rmsf = np.sqrt(rmsf/N)



Now we can plot RMSF

.. py-exec::
    :context-id: RMSD
    :discard-context:

    import matplotlib.pyplot as plt

    plt.figure(dpi=150)
    plt.step(range(len(rmsf)), rmsf, where="mid")
    plt.ylabel("RMSF, $\AA$")
    plt.grid(color="#CCCCCC",lw=0.1)

    def to_label(a):
        from  Bio.PDB.Polypeptide import three_to_one
        if a.rId.serial%5==0:
            return "%s\n%d"%(three_to_one(a.rName.str), a.rId.serial)
        else:
            return "%s"%(three_to_one(a.rName.str))

    plt.xticks(range(len(rmsf)),
               [ to_label(a) for a in first_chain_ca],
               rotation=0,fontsize="x-small")

    # can't show picture here, uncomment next line to see result
    # plt.show()




