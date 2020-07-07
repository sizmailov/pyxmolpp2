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

namespace xmol::proxy {

/** @brief Lightweight XYZ reference
 * */
class CoordRef {
public:
  CoordRef(const CoordRef& rhs) = default;
  CoordRef(CoordRef&& rhs) noexcept = default;
  CoordRef& operator=(const CoordRef& rhs) = default;
  CoordRef& operator=(CoordRef&& rhs) noexcept = default;

  CoordRef& set(const XYZ& value) {
    *m_coord = value;
    return *this;
  }

  [[nodiscard]] double x() const { return m_coord->x(); };
  CoordRef& x(double value) {
    m_coord->set_x(value);
    return *this;
  }
  [[nodiscard]] double y() const { return m_coord->y(); };
  CoordRef& y(double value) {
    m_coord->set_y(value);
    return *this;
  }
  [[nodiscard]] double z() const { return m_coord->z(); };
  CoordRef& z(double value) {
    m_coord->set_z(value);
    return *this;
  }

  inline CoordRef& operator+=(const XYZ& rhs) {
    m_coord->operator+=(rhs);
    return *this;
  }

  inline CoordRef& operator-=(const XYZ& rhs) {
    m_coord->operator-=(rhs);
    return *this;
  }

  inline CoordRef& operator*=(double rhs) {
    m_coord->operator*=(rhs);
    return *this;
  }

  inline CoordRef& operator/=(double rhs) {
    m_coord->operator/=(rhs);
    return *this;
  }

  inline double len2() const { return m_coord->len2(); }

  inline double len() const { return m_coord->len(); }

  inline double dot(const XYZ& b) const { return m_coord->dot(b); }

  inline XYZ cross(const XYZ& b) const { return XYZ(m_coord->cross(b)); }

  inline double distance(const XYZ& b) const { return m_coord->distance(b); }
  inline double distance2(const XYZ& b) const { return m_coord->distance2(b); }

  /// Check if references point to same data
  bool operator!=(const CoordRef& rhs) const {
    return m_coord != rhs.m_coord; // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  bool operator==(const CoordRef& rhs) const {
    return m_coord == rhs.m_coord; // comparing only one pair of pointers since they always must be in sync
  }

  operator const XYZ&() const { return *m_coord; }
  const XYZ::Vector_t& _eigen() const { return m_coord->_eigen(); }
  XYZ::Vector_t& _eigen() { return m_coord->_eigen(); }

protected:
  XYZ* m_coord = nullptr;

private:
  friend AtomSelection;
  friend Frame;
  friend MoleculeSelection;
  friend CoordSelection;
  friend ProxySpan<CoordRef, XYZ>;
  friend ResidueSelection;
  friend Selection<CoordRef>;
  friend smart::CoordSmartSelection;
  explicit CoordRef(XYZ& coord);

  CoordRef(XYZ* ptr, XYZ*);
  void advance() { ++m_coord; }
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

  /// Index of the molecule in frame
  [[nodiscard]] MoleculeIndex index() const noexcept;

  /// Parent frame
  Frame& frame() { return *m_molecule->frame; };

  /// Parent frame
  const Frame& frame() const { return *m_molecule->frame; };

  /// Residues of the molecule
  ResidueSpan residues() { return ResidueSpan{m_molecule->residues}; }

  /// Atoms of the molecule
  AtomSpan atoms() {
    if (empty())
      return {};
    assert(m_molecule->residues.m_begin);
    assert(m_molecule->residues.m_end);
    return AtomSpan(m_molecule->residues.m_begin->atoms.m_begin,
                       (m_molecule->residues.m_begin + size() - 1)->atoms.m_end);
  }

  /// Atom coordinates of the molecule
  CoordSpan coords() { return atoms().coords(); }

  /// Create smart reference from this
  smart::MoleculeSmartRef smart();

  /// Check if references point to same data
  bool operator!=(const MoleculeRef& rhs) const { return m_molecule != rhs.m_molecule; }
  bool operator==(const MoleculeRef& rhs) const { return m_molecule == rhs.m_molecule; }

  std::optional<proxy::ResidueRef> operator[](const ResidueId& id);
  std::optional<proxy::ResidueRef> operator[](residueSerial_t id);

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
  friend MoleculeSpan;
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

  /// Index of the residue in frame (0-based)
  [[nodiscard]] ResidueIndex index() const noexcept;

  /// Parent molecule
  MoleculeRef molecule() { return MoleculeRef(*m_residue->molecule); }

  /// Parent frame
  Frame& frame() { return *m_residue->molecule->frame; }

  /// Parent frame
  const Frame& frame() const { return *m_residue->molecule->frame; }

  /// Atoms of the residue
  AtomSpan atoms() { return AtomSpan{m_residue->atoms}; }

  /// Atom coordinates of the molecule
  CoordSpan coords() { return atoms().coords(); }

  /// Next residue in the molecule
  std::optional<ResidueRef> next() {
    if (m_residue + 1 < m_residue->molecule->residues.begin() + molecule().size()) {
      return ResidueRef(*(m_residue + 1));
    }
    return {};
  }

  /// Previous residue in the molecule
  std::optional<ResidueRef> prev() {
    if (m_residue - 1 >= m_residue->molecule->residues.begin()) {
      return ResidueRef(*(m_residue - 1));
    }
    return {};
  }

  /// Get children atom by name
  std::optional<AtomRef> operator[](const AtomName& name);
  std::optional<proxy::AtomRef> operator[](const char* name);
  std::optional<proxy::AtomRef> operator[](const std::string& name);


  /// Create smart reference from this
  smart::ResidueSmartRef smart();

  /// Check if references point to same data
  bool operator!=(const ResidueRef& rhs) const { return m_residue != rhs.m_residue; }
  bool operator==(const ResidueRef& rhs) const { return m_residue == rhs.m_residue; }



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
  friend ResidueSpan;
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

  /// Atom mass
  [[nodiscard]] float mass() const { return m_atom->mass; };
  AtomRef& mass(float value) {
    m_atom->mass = value;
    return *this;
  }

  /// Van der Waals radius
  [[nodiscard]] float vdw_radius() const { return m_atom->vdw_radius; };
  AtomRef& vdw_radius(float value) {
    m_atom->vdw_radius = value;
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
  [[nodiscard]] const XYZ& r() const { return *m_coord; }
  AtomRef& r(const XYZ& value) {
    *m_coord = value;
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

  /// Index of the atom in frame (0-based)
  [[nodiscard]] AtomIndex index() const noexcept;

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
  XYZ* m_coord = nullptr;
  BaseAtom* m_atom = nullptr;

private:
  friend AtomSpan;
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
    ++m_coord;
  }
  AtomRef() = default; // constructs object in invalid state (with nullptrs)
};

template <> struct Selection<proxy::CoordRef>::LessThanComparator {
  bool operator()(const proxy::CoordRef& p1, const proxy::CoordRef& p2) { return p1.m_coord < p2.m_coord; }
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

inline std::string to_string(const MoleculeRef& mol) { return mol.name().str(); }

inline std::string to_string(const ResidueRef& res) {
  return to_string(const_cast<ResidueRef&>(res).molecule()) + "." + res.name().str() + "-" + to_string(res.id());
}

inline std::string to_string(const AtomRef& atom) {
  return to_string(const_cast<AtomRef&>(atom).residue()) + "." + atom.name().str();
}

} // namespace xmol::proxy