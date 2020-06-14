
def make_polyglycine(chain_lengths):
    from pyxmolpp2 import Frame, XYZ, ResidueId

    aid = 1
    rid = 1
    frame = Frame()
    for chainId, N in chain_lengths:
        c = frame.add_molecule()
        c.name = chainId
        for i in range(N):
            r = c.add_residue()
            r.name = "GLY"
            r.id = ResidueId(rid)

            rid += 1
            for aname in ["N", "H", "CA", "HA2", "HA3", "C", "O"]:
                a = r.add_atom()
                a.name = aname
                a.id = aid
                a.r = XYZ(1, 2, 3)
                aid += 1

    return frame