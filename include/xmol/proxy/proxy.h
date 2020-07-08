#pragma once
#include "../base.h"
#include "Selection.h"
#include "mixins/AtomRefMixin.h"
#include "mixins/MoleculeRefMixin.h"
#include "mixins/ResidueRefMixin.h"
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

class MoleculeConstRef : public MoleculeGettersMixin<MoleculeConstRef> {
public:
  constexpr MoleculeConstRef(const MoleculeConstRef& rhs) = default;
  constexpr MoleculeConstRef(MoleculeConstRef&& rhs) noexcept = default;
  constexpr MoleculeConstRef& operator=(const MoleculeConstRef& rhs) = default;
  constexpr MoleculeConstRef& operator=(MoleculeConstRef&& rhs) noexcept = default;

private:
  BaseMolecule* m_molecule = nullptr;

  constexpr void check_invariants(const char*) const {};

  constexpr BaseMolecule* const mol_ptr() const { return m_molecule; }
  constexpr BaseMolecule* const& mol_ptr() { return m_molecule; }

  constexpr explicit MoleculeConstRef(BaseMolecule& Molecule) : m_molecule(&Molecule){};
  constexpr MoleculeConstRef(BaseMolecule* ptr, BaseMolecule*) : m_molecule(ptr){};
  constexpr void advance() { ++m_molecule; }
  constexpr MoleculeConstRef() = default; /// constructs object in invalid state (with nullptrs)

  friend MoleculeRef;
};

/** @brief Lightweight molecule reference
 *
 * For ref-counting reference see @ref smart::MoleculeSmartRef
 * */
class MoleculeRef : public MoleculeSettersMixin<MoleculeRef> {
public:
  /// Index of the molecule in frame
  [[nodiscard]] MoleculeIndex index() const noexcept;

  /// Parent frame
  Frame& frame() { return *mol_ptr()->frame; };

  /// Parent frame
  const Frame& frame() const { return *mol_ptr()->frame; };

  /// Residues of the molecule
  ResidueSpan residues() { return ResidueSpan{mol_ptr()->residues}; }

  /// Atoms of the molecule
  AtomSpan atoms() {
    if (empty())
      return {};
    assert(mol_ptr()->residues.m_begin);
    assert(mol_ptr()->residues.m_end);
    return AtomSpan(mol_ptr()->residues.m_begin->atoms.m_begin,
                       (mol_ptr()->residues.m_begin + size() - 1)->atoms.m_end);
  }

  /// Atom coordinates of the molecule
  CoordSpan coords() { return atoms().coords(); }

  /// Create smart reference from this
  smart::MoleculeSmartRef smart();

  /// Check if references point to same data
  constexpr bool operator!=(const MoleculeRef& rhs) const { return mol_ptr() != rhs.mol_ptr(); }
  constexpr bool operator==(const MoleculeRef& rhs) const { return mol_ptr() == rhs.mol_ptr(); }

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
  MoleculeConstRef m_cref;

  constexpr void check_invariants(const char*) const {};
  constexpr BaseMolecule* mol_ptr() const { return m_cref.m_molecule; }
  constexpr BaseMolecule*& mol_ptr() { return m_cref.m_molecule; }

  friend AtomRef;
  friend AtomSelection;
  friend Frame;
  friend MoleculeSpan;
  friend MoleculeSelection;
  friend ProxySpan<MoleculeRef, BaseMolecule>;
  friend ResidueRef;
  friend ResidueSelection;
  friend Selection<MoleculeRef>::LessThanComparator;
  friend ResidueGettersMixin<ResidueRef>;

  friend MoleculeSettersMixin<MoleculeRef>;
  friend MoleculeGettersMixin<MoleculeRef>;

  friend smart::MoleculeSmartRef;
  friend smart::MoleculeSmartSelection;

  constexpr explicit MoleculeRef(BaseMolecule& molecule) : m_cref(molecule){};
  constexpr MoleculeRef(BaseMolecule* ptr, BaseMolecule*) : m_cref(*ptr){};
  constexpr void advance() { m_cref.advance(); };
  constexpr MoleculeRef() = default; // constructs object in invalid state (with nullptrs)
};

class ResidueConstRef : public ResidueGettersMixin<ResidueConstRef> {
public:
  constexpr ResidueConstRef(const ResidueConstRef& rhs) = default;
  constexpr ResidueConstRef(ResidueConstRef&& rhs) noexcept = default;
  constexpr ResidueConstRef& operator=(const ResidueConstRef& rhs) = default;
  constexpr ResidueConstRef& operator=(ResidueConstRef&& rhs) noexcept = default;

private:
  BaseResidue* m_residue = nullptr;

  constexpr void check_invariants(const char*) const {};

  constexpr BaseResidue* const res_ptr() const { return m_residue; }
  constexpr BaseResidue* const& res_ptr() { return m_residue; }

  constexpr explicit ResidueConstRef(BaseResidue& residue) : m_residue(&residue){};
  constexpr ResidueConstRef(BaseResidue* ptr, BaseResidue*) : m_residue(ptr){};
  constexpr void advance() { ++m_residue; }
  constexpr ResidueConstRef() = default; /// constructs object in invalid state (with nullptrs)

  friend ResidueRef;
};

/** @brief Lightweight residue reference
 *
 * For ref-counting reference see @ref smart::ResidueSmartRef
 * */
class ResidueRef : public ResidueSettersMixin<ResidueRef> {
public:
  constexpr ResidueRef(const ResidueRef& rhs) = default;
  constexpr ResidueRef(ResidueRef&& rhs) noexcept = default;
  constexpr ResidueRef& operator=(const ResidueRef& rhs) = default;
  constexpr ResidueRef& operator=(ResidueRef&& rhs) noexcept = default;

  /// Index of the residue in frame (0-based)
  [[nodiscard]] ResidueIndex index() const noexcept;

  /// Atoms of the residue
  AtomSpan atoms() { return AtomSpan{res_ptr()->atoms}; }

  /// Atom coordinates of the molecule
  CoordSpan coords() { return atoms().coords(); }

  /// Get children atom by name
  std::optional<AtomRef> operator[](const AtomName& name);
  std::optional<proxy::AtomRef> operator[](const char* name);
  std::optional<proxy::AtomRef> operator[](const std::string& name);


  /// Create smart reference from this
  smart::ResidueSmartRef smart();

  /// Check if references point to same data
  bool operator!=(const ResidueRef& rhs) const { return res_ptr() != rhs.res_ptr(); }
  bool operator==(const ResidueRef& rhs) const { return res_ptr() == rhs.res_ptr(); }

  /// @brief Adds atom to the end of the reside and return its reference
  ///
  /// Invalidates all kinds of non-smart atom references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_atoms() call prevents references invalidation
  AtomRef add_atom();

private:
  ResidueConstRef c_ref;

  friend AtomRef;
  friend AtomSelection;
  friend Frame;
  friend MoleculeRef;
  friend MoleculeSelection;
  friend ProxySpan<ResidueRef, BaseResidue>;
  friend ResidueSpan;
  friend ResidueSelection;
  friend Selection<ResidueRef>::LessThanComparator;

  friend AtomGettersMixin<AtomRef>;
  friend AtomGettersMixin<smart::AtomSmartRef>;

  friend ResidueGettersMixin<ResidueRef>;
  friend ResidueSettersMixin<ResidueRef>;
  friend smart::ResidueSmartRef;
  friend smart::ResidueSmartSelection;

  constexpr void check_invariants(const char*) const {};

  constexpr BaseResidue* res_ptr() const { return c_ref.m_residue; }
  constexpr BaseResidue*& res_ptr() { return c_ref.m_residue; }

  constexpr explicit ResidueRef(BaseResidue& residue) : c_ref(residue){};

  constexpr ResidueRef(BaseResidue* ptr, BaseResidue*) : c_ref(*ptr){};
  constexpr void advance() { c_ref.advance(); }
  constexpr ResidueRef() = default; /// constructs object in invalid state (with nullptrs)
};

class AtomConstRef : public AtomGettersMixin<AtomConstRef> {
public:
  constexpr AtomConstRef(const AtomConstRef& rhs) = default;
  constexpr AtomConstRef(AtomConstRef&& rhs) noexcept = default;
  constexpr AtomConstRef& operator=(const AtomConstRef& rhs) = default;
  constexpr AtomConstRef& operator=(AtomConstRef&& rhs) noexcept = default;

private:
  XYZ* m_coord = nullptr;
  BaseAtom* m_atom = nullptr;

  constexpr XYZ* const coord_ptr() const { return m_coord; }
  constexpr XYZ* const& coord_ptr() { return m_coord; }
  constexpr BaseAtom* const atom_ptr() const { return m_atom; }
  constexpr BaseAtom* const& atom_ptr() { return m_atom; }

  constexpr void check_invariants(const char*) const {};

  constexpr void advance() {
    ++m_atom;
    ++m_coord;
  }

  constexpr AtomConstRef() = default; // constructs object in invalid state (with nullptrs)

  explicit AtomConstRef(BaseAtom& atom);
  AtomConstRef(BaseAtom* ptr, BaseAtom* end);

  friend AtomRef;
};

/** @brief Lightweight atom reference
 *
 * Provides access to @ref XYZ and @ref BaseAtom parts of atom
 *
 * For ref-counting reference see @ref smart::AtomSmartRef
 * */
class AtomRef : public AtomSettersMixin<AtomRef> {
public:
  constexpr AtomRef(const AtomRef& rhs) = default;
  constexpr AtomRef(AtomRef&& rhs) noexcept = default;
  constexpr AtomRef& operator=(const AtomRef& rhs) = default;
  constexpr AtomRef& operator=(AtomRef&& rhs) noexcept = default;

  using AtomSettersMixin::frame;
  /// Parent frame
  Frame& frame() { return *atom_ptr()->residue->molecule->frame; };

  /// Parent frame
  const Frame& frame() const { return *atom_ptr()->residue->molecule->frame; }

  /// Index of the atom in frame (0-based)
  [[nodiscard]] AtomIndex index() const noexcept;

  /// Create smart reference from this
  smart::AtomSmartRef smart();

  /// Check if references point to same data
  constexpr bool operator!=(const AtomRef& rhs) const {
    return atom_ptr() != rhs.atom_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

  /// Check if references point to same data
  constexpr bool operator==(const AtomRef& rhs) const {
    return atom_ptr() == rhs.atom_ptr(); // comparing only one pair of pointers since they always must be in sync
  }

private:
  AtomConstRef m_cref;

  [[nodiscard]] constexpr XYZ* coord_ptr() const { return m_cref.m_coord; }
  [[nodiscard]] constexpr XYZ*& coord_ptr() { return m_cref.m_coord; }
  [[nodiscard]] constexpr BaseAtom* atom_ptr() const { return m_cref.m_atom; }
  [[nodiscard]] constexpr BaseAtom*& atom_ptr() { return m_cref.m_atom; }
  constexpr void check_invariants(const char*) const {};

  friend AtomGettersMixin<AtomRef>;
  friend AtomSettersMixin<AtomRef>;
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
  explicit AtomRef(BaseAtom& atom) : m_cref(atom){}

  AtomRef(BaseAtom* ptr, BaseAtom* end) : m_cref(ptr, end) {}
  void advance() { m_cref.advance(); }
  constexpr AtomRef() = default; // constructs object in invalid state (with nullptrs)
};

template <> struct Selection<proxy::CoordRef>::LessThanComparator {
  bool operator()(const proxy::CoordRef& p1, const proxy::CoordRef& p2) { return p1.m_coord < p2.m_coord; }
};

template <> struct Selection<proxy::AtomRef>::LessThanComparator {
  bool operator()(const proxy::AtomRef& p1, const proxy::AtomRef& p2) { return p1.atom_ptr() < p2.atom_ptr(); }
};

template <> struct Selection<proxy::ResidueRef>::LessThanComparator {
  bool operator()(const proxy::ResidueRef& p1, const proxy::ResidueRef& p2) { return p1.res_ptr() < p2.res_ptr(); }
};

template <> struct Selection<proxy::MoleculeRef>::LessThanComparator {
  bool operator()(const proxy::MoleculeRef& p1, const proxy::MoleculeRef& p2) { return p1.mol_ptr() < p2.mol_ptr(); }
};

inline std::string to_string(const MoleculeRef& mol) { return mol.name().str(); }

inline std::string to_string(const ResidueRef& res) {
  return to_string(const_cast<ResidueRef&>(res).molecule()) + "." + res.name().str() + "-" + to_string(res.id());
}

inline std::string to_string(const AtomRef& atom) {
  return to_string(const_cast<AtomRef&>(atom).residue()) + "." + atom.name().str();
}

} // namespace xmol::proxy