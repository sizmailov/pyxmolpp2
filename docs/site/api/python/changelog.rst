Change log
##########

.. role:: py(code)
    :language: py

.. role:: text-dim
    :class: m-text m-dim

:ref-prefix:
    pyxmolpp2

next:
  - Added nice `__str__` representation for elements and selections
  - New: read ``time`` property from ``.xtc`` files
  - Changed signature of :ref:`TrajectoryInputFile.read_frame`
  - Allow ``*args`` in ``.is_in()`` method of predicate generators (e.g. :py:`aName.is_in("CA", "CB")`)
  - Fixed :py:`x in {x} == False` bug (:ref:`Atom`, :ref:`Residue`, :ref:`Molecule`, :ref:`ResidueId` were affected)

v1.3:
  - Removed: ``Trajectory.Frame``
  - New: Added :ref:`Frame.index`, :ref:`Frame.time` (note: time is not read from trajectory files (yet))

v1.2:
  - New: Support for ``.xtc`` trajectory format (see :ref:`GromacsXtcFile`, :ref:`XtcWriter`)
  - New: Added :ref:`Atom.vdw_radius` property

v1.1:
  - New: Trajectory reads :ref:`Frame.cell` from input formats (:ref:`AmberNetCDF` | :ref:`PdbFile`)
  - Renamed ``AngleValue.{fabs -> abs}``
  - Renamed ``TorsionAngle.{set -> rotate_to}``
  - Fixed :ref:`TrajectoryInputFile` trampoline
  - Added basic ``CRYST1`` pdb record support

v1.0.0:
  - Added :ref:`pipe.AssembleQuaternaryStructure`, :ref:`pipe.ScaleUnitCell`
  - Changed internal representation of frame data to flat layout
  - Introduced lightweight continuous "spans"
  - Added coordinates selections
  - ``0.x`` submodules are flattened
  - ``0.x`` ``Chain`` renamed to ``Molecule``
  - Removed ``VectorXYZ`` in favor of numpy arrays
  - Removed ``AtomName`` and others are replaced with bare strings
  - :ref:`XYZ` become immutable
  - ``LatticeVectors`` united with ``BestShiftFinder`` in :ref:`UnitCell`
  - ``PdbRecords`` and others are removed from python API, replaced with :ref:`PdbFile.Dialect`
  - Free functions ``distance``, ``angle``, ``dihedral_angle`` replaced with corresponding methods of :ref:`XYZ`
  - ``asAtoms`` properties are renamed to ``atoms``, same for ``residues`` and ``molecules``
  - :ref:`PdbFile` read files in AMBER compatible mode by default
  - Introduced "pipe" trajectory processing, see :ref:`std:doc:overview`
  - Added weighted versions of alignment and rmsd calculation
  - A number of minor improvements

v0.10.4:
  - Fix doc generation
  - Remove UB in ``remove_if()`` function

v0.10.3:
  - License software under MIT license

v0.10.2:
  - Fix typos in doc

v0.10.1:
  - Fix cmake module path

v0.10.0:
  - Add :ref:`calc_autocorr_order_2_PRE()`

v0.9.0:
  - Add support for netcdf (.nc) Amber trajectory files

v0.8.2:
  - Doc: improve doc generation, fix typos

v0.8.1:
  - Fix: inertia tensor off diagonal elements opposite sign

v0.8.0:
  - Allow python buffers as arguments of ``.to_pdb``
  - Add ``.to_pdb`` method to FRCA and selections
  - Update dependencies
  - CI: migrate to travis-ci.com, add missed gtests
  - sanitize acos/asin inputs

v0.7.1:
  - Fix ``Rotation3d.axis()``
v0.7.0:
  - Add Solvent Accessible Surface Area calculation procedure :ref:`calc_sasa`
  - Allow numpy-style array indexing for selections, see ``array_indexing``

v0.6.1:
  - Remove redundant quotes from ``str(AtomName)``
  - Explicitly set locale to avoid commas in float numbers
  - Update docs
v0.6.0:
  - Fix alignment shortcuts (``AtomSelection.align_to``, :ref:`AtomSelection.alignment_to`)
  - Remove ``AtomSelection.alignment``
  - Add ``VectorXYZ.to_numpy``, ``VectorXYZ.from_numpy``
  - Show detailed message or read error in ``PdbFile.get_frames``
  - Use live examples in docs (do avoid divergence with sources)
v0.5.0:
  - Add convenient methods to :ref:`AtomSelection`, ``VectorXYZ``
  - Allow ``Rotation3d`` construction from numpy array
  - Allow :ref:`AtomSelection` construction python lists of :ref:`Atom`
  - Make :ref:`Atom` properties writable
  - Move PyCharm stubs to a separate package (:link:`https://github.com/sizmailov/pyxmolpp2-stubs`)
  - Automate PyPI upload and github-pages generation
v0.4.0:
  - Allow ``AtomSelection.transform`` to accept ``Transformation3d``, ``UniformScale3d`` ,
    ``Rotation3d`` and ``Translation3d``
  - Support ``.dat`` files created with charmm force field
v0.3.0:
  - Add :ref:`calc_autocorr_order_2`

v0.2.0:
  - Change ``PdbFile`` interface
  - Fix ``calc_alignment``
  - Support selection slicing
  - Implement predicates on C++ side
  - Implement convenient predicate generators (:ref:`aName`, :ref:`rId`, etc)
  - Fix representation of :ref:`XYZ`
  - Add conversion method :ref:`XYZ` -> :ref:`numpy.ndarray`
  - Provide access to transformation matrices for ``Transformation3d`` and ``Rotation3d``
  - Add equality operations to Atom/Residue/Chain names
  - Minor improvements
  - Update docs


v0.1.1:
  - Bug fix: name/index lookup in Frame and Chain

v0.1.0:
  - Add set operations for selections
  - Optimize selections construction
  - Add torsion angles with definitions for protein chains
  - Add basic operations on crystal/periodic lattice
  - Wrap raw C++ references to avoid segfaults
  - Support partial frame updates in trajectory traverse
  - Drop compiler requirements from C++14 to C++11
  - Test against python 2.7 and 3.5
  - Version documentation on gh-pages
  - Bug fix
