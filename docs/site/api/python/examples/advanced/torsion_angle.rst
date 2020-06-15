Torsion Angles
^^^^^^^^^^^^^^

:ref:`TorsionAngle` represents a tuple of 4 atoms (``a-b-c-d``) that correspond to dihedral angle.
In addition it might be able to rotate part of structure around ``b-c`` bond (where ``a`` is static, ``d`` is moving).

.. py-exec::
    :hide-code:
    :context-id: torsion_angle

    import os
    from pyxmolpp2 import PdbFile

    pdb_filename = os.path.join(os.environ["TEST_DATA_PATH"], "pdb/rcsb/1UBQ.pdb")
    pdb_file = PdbFile(pdb_filename)

    frame = pdb_file.frames()[0]

For standard protein residues angles can be constructed using :ref:`TorsionAngleFactory`:

.. py-exec::
    :context-id: torsion_angle

    from pyxmolpp2.polymer import TorsionAngleFactory
    from pyxmolpp2.geometry import Degrees

    residue48 = frame.molecules[0][48]
    print(residue48)

    psi_48 = TorsionAngleFactory.psi(residue48)
    print(psi_48)
    print(psi_48.value().degrees)


.. block-warning::
    Check returned values

    :ref:`TorsionAngleFactory` may return ``None`` if such angle does not exist

.. py-exec::
    :context-id: torsion_angle

    # first residue don't have omega angle
    print(TorsionAngleFactory.omega(frame.asResidues[0]))

Let's rotate ``psi_48`` angle:

.. py-exec::
    :context-id: torsion_angle

    # Residues 49-76 are affected by this rotation
    psi_48.set(Degrees(150))
    print(psi_48.value().degrees)


Torsion angle constructor allow two forms:
  1. Read-only torsion angle
  2. Read-write torsion angle

.. py-exec::
    :context-id: torsion_angle


    from pyxmolpp2.polymer import TorsionAngle, AtomName, Atom

    r1 = frame.asResidues[1]
    r2 = frame.asResidues[2]

    # Let's create a read-only phi of residue 2
    phi_2_ro = TorsionAngle(r1[AtomName("C")],
                            r2[AtomName("N")],
                            r2[AtomName("CA")],
                            r2[AtomName("C")],
                            )

    # Check against factory angle:
    assert phi_2_ro.value().degrees == TorsionAngleFactory.phi(r2).value().degrees

Attempt to set read-only agle will lead to ``RuntimeError``:

.. py-exec::
    :context-id: torsion_angle
    :raises: RuntimeError

    phi_2_ro.set(Degrees(-130))


To make :ref:`TorsinAngle` writeable one need to pass to constructor a selector function
which returns a selection of affected atoms by our torsion angle

.. py-exec::
    :context-id: torsion_angle
    :discard-context:

    def affected_phi_atoms(a: Atom, b: Atom, c: Atom, d: Atom):
        from pyxmolpp2.polymer import rId
        return a.chain.asResidues.filter(rId > a.rId).asAtoms


    phi_2_rw = TorsionAngle(r1[AtomName("C")],
                            r2[AtomName("N")],
                            r2[AtomName("CA")],
                            r2[AtomName("C")],
                            affected_phi_atoms
                            )

    phi_2_rw.set(Degrees(-130))

    print(phi_2_ro.value().degrees)
    print(phi_2_rw.value().degrees)


