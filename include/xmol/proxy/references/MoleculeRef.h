#pragma once

#include "ConstMoleculeRef.h"

namespace xmol::proxy {

/** @brief Lightweight molecule reference
 *
 * For ref-counting reference see @ref smart::MoleculeSmartRef
 * */
class MoleculeRef {
public:
  [[nodiscard]] constexpr MoleculeName name() const {
    check_invariants("name");
    return mol_ptr()->name;
  }

  template <int N>[[maybe_unused]] constexpr const MoleculeRef& name(const char (&value)[N]) const& {
    check_invariants("name");
    mol_ptr()->name = MoleculeName(value);
    return *this;
  }

  template <int N>[[maybe_unused]] constexpr MoleculeRef name(const char (&value)[N]) const&& {
    check_invariants("name");
    mol_ptr()->name = MoleculeName(value);
    return *this;
  }

  [[maybe_unused]] const MoleculeRef& name(const std::string& value) const& {
    check_invariants("name");
    mol_ptr()->name = MoleculeName(value);
    return *this;
  }

  [[maybe_unused]] MoleculeRef name(const std::string& value) const&& {
    check_invariants("name");
    mol_ptr()->name = MoleculeName(value);
    return *this;
  }

  [[maybe_unused]] constexpr const MoleculeRef& name(const MoleculeName& value) const& {
    check_invariants("name");
    mol_ptr()->name = value;
    return *this;
  }

  [[maybe_unused]] constexpr MoleculeRef name(const MoleculeName& value) const&& {
    check_invariants("name");
    mol_ptr()->name = value;
    return *this;
  }

  [[nodiscard]] constexpr bool empty() const {
    check_invariants("empty");
    return mol_ptr()->residues.m_begin == mol_ptr()->residues.m_end;
  }

  [[nodiscard]] constexpr size_t size() const {
    check_invariants("size");
    return mol_ptr()->residues.m_end - mol_ptr()->residues.m_begin;
  }

  [[nodiscard]] constexpr Frame& frame() const {
    check_invariants("frame");
    return *mol_ptr()->frame;
  }

  /// Index of the molecule in frame
  [[nodiscard]] MoleculeIndex index() const;

  /// Residues of the molecule
  [[nodiscard]] ResidueSpan residues() const { return ResidueSpan{mol_ptr()->residues}; }

  /// Atoms of the molecule
  [[nodiscard]] AtomSpan atoms() const {
    if (empty())
      return {};
    assert(mol_ptr()->residues.m_begin);
    assert(mol_ptr()->residues.m_end);
    return AtomSpan(mol_ptr()->residues.m_begin->atoms.m_begin,
                    (mol_ptr()->residues.m_begin + size() - 1)->atoms.m_end);
  }

  /// Atom coordinates of the molecule
  [[nodiscard]] CoordSpan coords() const { return atoms().coords(); }

  /// Create smart reference from this
  [[nodiscard]] MoleculeSmartRef smart() const;

  /// Check if references point to same data
  constexpr bool operator!=(const MoleculeRef& rhs) const { return mol_ptr() != rhs.mol_ptr(); }
  constexpr bool operator==(const MoleculeRef& rhs) const { return mol_ptr() == rhs.mol_ptr(); }

  std::optional<proxy::ResidueRef> operator[](const ResidueId& id) const ;
  std::optional<proxy::ResidueRef> operator[](ResidueIdSerial id) const ;

  /// @brief Adds residue to the end of the molecule and return its reference
  ///
  /// Invalidates all kinds of non-smart residue references including proxy::ResidueRef, proxy::ResidueRefSpan and
  /// proxy::ResidueSelection
  ///
  /// Appropriate Frame::reserve_residues() call prevents references invalidation
  [[nodiscard]] ResidueRef add_residue() const;

private:
  ConstMoleculeRef m_cref;

  constexpr void check_invariants(const char*) const {};
  [[nodiscard]] constexpr BaseMolecule* mol_ptr() const { return m_cref.m_molecule; }

  friend AtomRef;
  friend AtomSelection;
  friend Frame;
  friend MoleculeSpan;
  friend MoleculeSelection;
  friend ProxySpan<MoleculeRef, BaseMolecule>;
  friend ResidueRef;
  friend ResidueSelection;
  friend Selection<MoleculeRef>::LessThanComparator;

  template<typename, typename>
  friend class proxy::api::AtomAPI;
  template<typename, typename>
  friend class proxy::api::ResidueAPI;

  friend MoleculeSmartRef;
  friend smart::MoleculeSmartSelection;

  constexpr explicit MoleculeRef(BaseMolecule& molecule) : m_cref(molecule){};
  constexpr MoleculeRef(BaseMolecule* ptr, BaseMolecule*) : m_cref(*ptr){};
  constexpr void advance() { m_cref.advance(); };
  void rebase(BaseMolecule* from, BaseMolecule* to) { m_cref.rebase(from, to); }
  constexpr MoleculeRef() = default; // constructs object in invalid state (with nullptrs)
};

inline std::string to_string(const MoleculeRef& mol) { return mol.name().str(); }

template <> struct Selection<proxy::MoleculeRef>::LessThanComparator {
  bool operator()(const proxy::MoleculeRef& p1, const proxy::MoleculeRef& p2) { return p1.mol_ptr() < p2.mol_ptr(); }
};

} // namespace xmol::proxy