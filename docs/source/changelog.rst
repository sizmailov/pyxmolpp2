----------
Change log
----------
v0.4.0:
  - Allow :py:func:`pyxmolpp2.polymer.AtomSelection.transform` to accept :py:class:`~pyxmolpp2.geometry.Transformation3d`, :py:class:`~pyxmolpp2.geometry.UniformScale3d` ,
    :py:class:`~pyxmolpp2.geometry.Rotation3d` and :py:class:`~pyxmolpp2.geometry.Translation3d`
  - Support ``.dat`` files created with charmm force field
v0.3.0:
  - Add :py:func:`~pyxmolpp2.geometry.calc_autocorr_order_2`

v0.2.0:
  - Change :py:class:`~pyxmolpp2.pdb.PdbFile` interface
  - Fix :py:func:`~pyxmolpp2.geometry.calc_alignment`
  - Support selection slicing
  - Implement predicates on C++ side
  - Implement convenient predicate generators (:py:data:`~pyxmolpp2.polymer.aName`, :py:data:`~pyxmolpp2.polymer.rId`, etc)
  - Fix representation of :py:class:`~pyxmolpp2.geometry.XYZ`
  - Add conversion method :py:class:`~pyxmolpp2.geometry.XYZ` -> :py:class:`numpy.array`
  - Provide access to transformation matrices for :py:class:`~pyxmolpp2.geometry.Transformation3d` and :py:class:`~pyxmolpp2.geometry.Rotation3d`
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
