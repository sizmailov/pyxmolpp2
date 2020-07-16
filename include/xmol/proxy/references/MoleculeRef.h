#pragma once

#include "ConstMoleculeRef.h"

namespace xmol::proxy {

/** @brief Lightweight molecule reference
 *
 * For ref-counting reference see @ref smart::MoleculeSmartRef
 * */
class MoleculeRef : public MoleculeSettersMixin<MoleculeRef> {
public:
  /// Index of the molecule in frame
  [[nodiscard]] MoleculeIndex index() const noexcept;

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
  ConstMoleculeRef m_cref;

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

inline std::string to_string(const MoleculeRef& mol) { return mol.name().str(); }

template <> struct Selection<proxy::MoleculeRef>::LessThanComparator {
  bool operator()(const proxy::MoleculeRef& p1, const proxy::MoleculeRef& p2) { return p1.mol_ptr() < p2.mol_ptr(); }
};

} // namespace xmol::proxy