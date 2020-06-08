#pragma once
#include "../base.h"
#include "Selection.h"
#include "spans.h"

/** @file
 * Lightweight proxy reference classes for Atom, Residue and Molecule.
 *
 * Instances are invalidated on insertion/deletion of corresponding entity to parent frame.
 * Access to invalidated instance is prohibited and most likely would lead to immediate SEGFAULT (if you are lucky)
 *
 * */

namespace xmol::v1::proxy {

/** @brief Lightweight XYZ reference
 * */
class CoordRef {
public:
  CoordRef(const CoordRef& rhs) = default;
  CoordRef(CoordRef&& rhs) noexcept = default;
  CoordRef& operator=(const CoordRef& rhs) = default;
  CoordRef& operator=(CoordRef&& rhs) noexcept = default;

  CoordRef& set(const XYZ& value) {
    *m_coords = value;
    return *this;
  }

  [[nodiscard]] double x() const { return m_coords->x(); };
  CoordRef& x(double value) {
    m_coords->set_x(value);
    return *this;
  }
  [[nodiscard]] double y() const { return m_coords->y(); };
  CoordRef& y(double value) {
    m_coords->set_y(value);
    return *this;
  }
  [[nodiscard]] double z() const { return m_coords->z(); };
  CoordRef& z(double value) {
    m_coords->set_z(value);
    return *this;
  }

  /// Check if references point to same data
  bool operator!=(const CoordRef& rhs) const {
    return m_coords != rhs.m_coords; // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  bool operator==(const CoordRef& rhs) const {
    return m_coords == rhs.m_coords; // comparing only one pair of pointers since they always must be in sync
  }

  operator const XYZ&() const { return *m_coords; }
  const Eigen::Vector3d& _eigen() const { return m_coords->_eigen(); }
  Eigen::Vector3d& _eigen() { return m_coords->_eigen(); }

protected:
  XYZ* m_coords = nullptr;

private:
  friend Frame;
  friend ProxySpan<CoordRef, XYZ>;
  explicit CoordRef(XYZ& coord);

  CoordRef(XYZ* ptr, XYZ*);
  void advance() { ++m_coords; }
  CoordRef() = default; // constructs object in invalid state (with nullptrs)
};

/** @brief Lightweight molecule reference
 *
 * For ref-counting reference see @ref smart::MoleculeSmartRef
 * */
class MoleculeRef {
public:
  /// Molecule name
  [[nodiscard]] const MoleculeName& name() const {
    assert(m_molecule);
    assert(m_molecule->frame);
    return m_molecule->name;
  }

  MoleculeRef& name(const MoleculeName& name) {
    assert(m_molecule);
    assert(m_molecule->frame);
    m_molecule->name = name;
    return *this;
  }

  MoleculeRef& name(const char* name) {
    assert(m_molecule);
    assert(m_molecule->frame);
    m_molecule->name = MoleculeName(name);
    return *this;
  }

  MoleculeRef& name(const std::string& name) {
    assert(m_molecule);
    assert(m_molecule->frame);
    m_molecule->name = MoleculeName(name);
    return *this;
  }

  /// Check if molecule has no residues
  [[nodiscard]] bool empty() const {
    assert(m_molecule);
    assert(m_molecule->frame);
    return m_molecule->residues.m_begin == m_molecule->residues.m_end;
  }

  /// Number of residues in molecule
  [[nodiscard]] size_t size() const {
    assert(m_molecule);
    assert(m_molecule->frame);
    return m_molecule->residues.m_end - m_molecule->residues.m_begin;
  }

  /// Parent frame
  Frame& frame() { return *m_molecule->frame; };

  /// Parent frame
  const Frame& frame() const { return *m_molecule->frame; };

  /// Residues of the molecule
  ResidueRefSpan residues() { return ResidueRefSpan{m_molecule->residues}; }

  /// Atoms of the molecule
  AtomRefSpan atoms() {
    if (empty())
      return {};
    assert(m_molecule->residues.m_begin);
    assert(m_molecule->residues.m_end);
    return AtomRefSpan(m_molecule->residues.m_begin->atoms.m_begin,
                       (m_molecule->residues.m_begin + size() - 1)->atoms.m_end);
  }

  /// Create smart reference from this
  smart::MoleculeSmartRef smart();

  /// Check if references point to same data
  bool operator!=(const MoleculeRef& rhs) const { return m_molecule != rhs.m_molecule; }

  /// @brief Adds residue to the end of the molecule and return its reference
  ///
  /// Invalidates all kinds of non-smart residue references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_residues() call prevents references invalidation
  ResidueRef add_residue();

private:
  friend AtomRef;
  friend AtomSelection;
  friend Frame;
  friend MoleculeRefSpan;
  friend MoleculeSelection;
  friend ProxySpan<MoleculeRef, BaseMolecule>;
  friend ResidueRef;
  friend ResidueSelection;
  friend Selection<MoleculeRef>::LessThanComparator;
  friend smart::MoleculeSmartRef;
  friend smart::MoleculeSmartSelection;
  BaseMolecule* m_molecule;
  explicit MoleculeRef(BaseMolecule& molecule) : m_molecule(&molecule){};
  MoleculeRef(BaseMolecule* ptr, BaseMolecule* end) : m_molecule(ptr){};
  void advance() { ++m_molecule; };
  MoleculeRef() = default; // constructs object in invalid state (with nullptrs)
};

/** @brief Lightweight residue reference
 *
 * For ref-counting reference see @ref smart::ResidueSmartRef
 * */
class ResidueRef {
public:
  /// Residue name
  [[nodiscard]] const ResidueName& name() const {
    assert(m_residue);
    return m_residue->name;
  }
  ResidueRef& name(const ResidueName& name) {
    assert(m_residue);
    m_residue->name = name;
    return *this;
  }

  ResidueRef& name(const char* name) {
    assert(m_residue);
    m_residue->name = ResidueName(name);
    return *this;
  }

  /// Residue id
  [[nodiscard]] const ResidueId& id() const { return m_residue->id; };
  ResidueRef& id(const ResidueId& value) {
    m_residue->id = value;
    return *this;
  }

  ResidueRef& id(int serial) {
    m_residue->id = serial;
    return *this;
  }

  /// Check if residue has no atoms
  [[nodiscard]] bool empty() const {
    assert(m_residue);
    return m_residue->atoms.m_begin == m_residue->atoms.m_end;
  }

  /// Number of atoms in the residue
  [[nodiscard]] size_t size() const {
    assert(m_residue);
    return m_residue->atoms.m_end - m_residue->atoms.m_begin;
  }

  /// Parent molecule
  MoleculeRef molecule() { return MoleculeRef(*m_residue->molecule); }

  /// Parent frame
  Frame& frame() { return *m_residue->molecule->frame; }

  /// Parent frame
  const Frame& frame() const { return *m_residue->molecule->frame; }

  /// Atoms of the residue
  AtomRefSpan atoms() { return AtomRefSpan{m_residue->atoms}; }

  /// Create smart reference from this
  smart::ResidueSmartRef smart();

  /// Check if references point to same data
  bool operator!=(const ResidueRef& rhs) const { return m_residue != rhs.m_residue; }

  /// @brief Adds atom to the end of the reside and return its reference
  ///
  /// Invalidates all kinds of non-smart atom references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_atoms() call prevents references invalidation
  AtomRef add_atom();

private:
  friend AtomRef;
  friend AtomSelection;
  friend Frame;
  friend MoleculeRef;
  friend MoleculeSelection;
  friend ProxySpan<ResidueRef, BaseResidue>;
  friend ResidueRefSpan;
  friend ResidueSelection;
  friend Selection<ResidueRef>::LessThanComparator;
  friend smart::ResidueSmartRef;
  friend smart::ResidueSmartSelection;
  explicit ResidueRef(BaseResidue& residue) : m_residue(&residue){};
  BaseResidue* m_residue = nullptr;
  ResidueRef(BaseResidue* ptr, BaseResidue* end) : m_residue(ptr){};
  void advance() { ++m_residue; }
  ResidueRef() = default; /// constructs object in invalid state (with nullptrs)
};

/** @brief Lightweight atom reference
 *
 * Provides access to @ref XYZ and @ref BaseAtom parts of atom
 *
 * For ref-counting reference see @ref smart::AtomSmartRef
 * */
class AtomRef {
public:
  AtomRef(const AtomRef& rhs) = default;
  AtomRef(AtomRef&& rhs) noexcept = default;
  AtomRef& operator=(const AtomRef& rhs) = default;
  AtomRef& operator=(AtomRef&& rhs) noexcept = default;

  /// Atom id
  [[nodiscard]] const AtomId& id() const { return m_atom->id; };
  AtomRef& id(const AtomId& value) {
    m_atom->id = value;
    return *this;
  }

  /// Atom name
  [[nodiscard]] const AtomName& name() const { return m_atom->name; };
  AtomRef& name(const AtomName& value) {
    m_atom->name = value;
    return *this;
  }

  AtomRef& name(const char* value) {
    m_atom->name = AtomName(value);
    return *this;
  }

  AtomRef& name(const std::string& value) {
    m_atom->name = AtomName(value);
    return *this;
  }

  /// Atom coordinates
  [[nodiscard]] const XYZ& r() const { return *m_coords; }
  AtomRef& r(const XYZ& value) {
    *m_coords = value;
    return *this;
  }

  /// Parent residue
  ResidueRef residue() { return ResidueRef(*m_atom->residue); }

  /// Parent molecule
  MoleculeRef molecule() { return MoleculeRef(*m_atom->residue->molecule); };

  /// Parent frame
  Frame& frame() { return *m_atom->residue->molecule->frame; };

  /// Parent frame
  const Frame& frame() const { return *m_atom->residue->molecule->frame; }

  /// Create smart reference from this
  smart::AtomSmartRef smart();

  /// Check if references point to same data
  bool operator!=(const AtomRef& rhs) const {
    return m_atom != rhs.m_atom; // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  bool operator==(const AtomRef& rhs) const {
    return m_atom == rhs.m_atom; // comparing only one pair of pointers since they always must be in sync
  }

protected:
  XYZ* m_coords = nullptr;
  BaseAtom* m_atom = nullptr;

private:
  friend AtomRefSpan;
  friend AtomSelection;
  friend Frame;
  friend MoleculeSelection;
  friend ProxySpan<AtomRef, BaseAtom>;
  friend ResidueRef;
  friend ResidueSelection;
  friend Selection<proxy::AtomRef>::LessThanComparator;
  friend smart::AtomSmartRef;
  friend smart::AtomSmartSelection;
  friend smart::AtomSmartSpan;
  explicit AtomRef(BaseAtom& atom);

  AtomRef(BaseAtom* ptr, BaseAtom* end);
  void advance() {
    ++m_atom;
    ++m_coords;
  }
  AtomRef() = default; // constructs object in invalid state (with nullptrs)
};

template <> struct Selection<proxy::AtomRef>::LessThanComparator {
  bool operator()(const proxy::AtomRef& p1, const proxy::AtomRef& p2) { return p1.m_atom < p2.m_atom; }
};

template <> struct Selection<proxy::ResidueRef>::LessThanComparator {
  bool operator()(const proxy::ResidueRef& p1, const proxy::ResidueRef& p2) { return p1.m_residue < p2.m_residue; }
};

template <> struct Selection<proxy::MoleculeRef>::LessThanComparator {
  bool operator()(const proxy::MoleculeRef& p1, const proxy::MoleculeRef& p2) { return p1.m_molecule < p2.m_molecule; }
};

} // namespace xmol::v1::proxy