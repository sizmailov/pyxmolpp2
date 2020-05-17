----------
Change log
----------

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
  - Add :ref:`pyxmolpp2.geometry.calc_autocorr_order_2_PRE()`

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
  - Fix :ref:`pyxmolpp2.geometry.Rotation3d.axis()`
v0.7.0:
  - Add Solvent Accessible Surface Area calculation procedure :ref:`pyxmolpp2.geometry.calc_sasa`, see :ref:`examples.sasa_calc`
  - Allow numpy-style array indexing for selections, see :ref:`examples.array_indexing`

v0.6.1:
  - Remove redundant quotes from ``str(AtomName)``
  - Explicitly set locale to avoid commas in float numbers
  - Update docs
v0.6.0:
  - Fix alignment shortcuts (:ref:`pyxmolpp2.polymer.AtomSelection.align_to`, :ref:`pyxmolpp2.polymer.AtomSelection.alignment_to`)
  - Remove :ref:`pyxmolpp2.polymer.AtomSelection.alignment`
  - Add :ref:`pyxmolpp2.geometry.VectorXYZ.to_numpy`, :ref:`pyxmolpp2.geometry.VectorXYZ.from_numpy`
  - Show detailed message or read error in :ref:`pyxmolpp2.pdb.PdbFile.get_frames`
  - Use live examples in docs (do avoid divergence with sources)
v0.5.0:
  - Add convenient methods to :ref:`pyxmolpp2.polymer.AtomSelection`, :ref:`pyxmolpp2.geometry.VectorXYZ`
  - Allow :ref:`pyxmolpp2.geometry.Rotation3d` construction from numpy array
  - Allow :ref:`pyxmolpp2.polymer.AtomSelection` construction python lists of :ref:`pyxmolpp2.polymer.Atom`
  - Make :ref:`pyxmolpp2.polymer.Atom` properties writable
  - Move PyCharm stubs to a separate package (:link:`https://github.com/sizmailov/pyxmolpp2-stubs`)
  - Automate PyPI upload and github-pages generation
v0.4.0:
  - Allow :ref:`pyxmolpp2.polymer.AtomSelection.transform` to accept :ref:`pyxmolpp2.geometry.Transformation3d`, :ref:`pyxmolpp2.geometry.UniformScale3d` ,
    :ref:`pyxmolpp2.geometry.Rotation3d` and :ref:`pyxmolpp2.geometry.Translation3d`
  - Support ``.dat`` files created with charmm force field
v0.3.0:
  - Add :ref:`pyxmolpp2.geometry.calc_autocorr_order_2`

v0.2.0:
  - Change :ref:`pyxmolpp2.pdb.PdbFile` interface
  - Fix :ref:`pyxmolpp2.geometry.calc_alignment`
  - Support selection slicing
  - Implement predicates on C++ side
  - Implement convenient predicate generators (:ref:`pyxmolpp2.polymer.aName`, :ref:`pyxmolpp2.polymer.rId`, etc)
  - Fix representation of :ref:`pyxmolpp2.geometry.XYZ`
  - Add conversion method :ref:`pyxmolpp2.geometry.XYZ` -> :ref:`numpy.ndarray`
  - Provide access to transformation matrices for :ref:`pyxmolpp2.geometry.Transformation3d` and :ref:`pyxmolpp2.geometry.Rotation3d`
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
